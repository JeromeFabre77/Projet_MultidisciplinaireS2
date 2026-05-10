// ============================================================================
// SERVICES
// ============================================================================

class GeoService {
    static EARTH_RADIUS_KM = 6371;
    static COVERAGE_RADIUS_KM = 10;

    static toRadians(degrees) {
        return (degrees * Math.PI) / 180;
    }

    static distanceInKm(lat1, lon1, lat2, lon2) {
        const dLat = this.toRadians(lat2 - lat1);
        const dLon = this.toRadians(lon2 - lon1);
        const a =
            Math.sin(dLat / 2) ** 2 +
            Math.cos(this.toRadians(lat1)) * Math.cos(this.toRadians(lat2)) *
            Math.sin(dLon / 2) ** 2;
        const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        return this.EARTH_RADIUS_KM * c;
    }

    static isCovered(city, hospitals) {
        return hospitals.some(hospital =>
            this.distanceInKm(
                Number(city.latitude),
                Number(city.longitude),
                Number(hospital.location.latitude),
                Number(hospital.location.longitude)
            ) <= this.COVERAGE_RADIUS_KM
        );
    }
}

class FormatService {
    static formatNumber(value) {
        return Number(value || 0).toLocaleString('fr-FR');
    }

    static formatBedRate(bedsPer1000) {
        return bedsPer1000.toFixed(2);
    }
}

class CityService {
    constructor(cities) {
        this.cities = cities;
    }

    findExact(lat, lon) {
        return this.cities.find(city =>
            Number(city.latitude) === lat && Number(city.longitude) === lon
        );
    }

    findNearest(lat, lon) {
        return this.cities
            .map(city => ({
                ...city,
                distance: GeoService.distanceInKm(
                    Number(city.latitude), Number(city.longitude), lat, lon
                )
            }))
            .sort((a, b) => a.distance - b.distance)[0];
    }

    findCityName(lat, lon) {
        const exact = this.findExact(lat, lon);
        if (exact) return exact.name;

        const nearest = this.findNearest(lat, lon);
        return nearest ? `${nearest.name} (approx.)` : 'Zone inconnue';
    }
}

// ============================================================================
// MODELS
// ============================================================================

class Department {
    constructor(code, name) {
        this.code = code;
        this.name = name;
        this.cities = [];
        this.hospitals = [];
    }

    addCity(city) {
        this.cities.push(city);
    }

    addHospital(cityName, bedCount) {
        this.hospitals.push({ cityName, bedCount });
    }

    getTotalPopulation() {
        return this.cities.reduce((sum, city) => sum + Number(city.population || 0), 0);
    }

    getTotalBeds() {
        return this.hospitals.reduce((sum, h) => sum + Number(h.bedCount || 0), 0);
    }

    getDesertMetrics(allHospitals) {
        const desertCities = this.cities.filter(city => !GeoService.isCovered(city, allHospitals));
        const desertPopulation = desertCities.reduce((sum, city) => sum + Number(city.population || 0), 0);
        return { count: desertCities.length, population: desertPopulation };
    }

    getBedRate() {
        const pop = this.getTotalPopulation();
        return pop > 0 ? (this.getTotalBeds() / pop) * 1000 : 0;
    }
}

// ============================================================================
// REPORT GENERATOR
// ============================================================================

class ReportGenerator {
    constructor(data) {
        this.data = data;
        this.cityService = new CityService(data.cities || []);
        this.departments = this.buildDepartments();
    }

    buildDepartments() {
        const deptMap = new Map();

        // Ajouter toutes les communes aux départements
        this.data.cities.forEach(city => {
            const code = city.dept_code;
            if (!deptMap.has(code)) {
                deptMap.set(code, new Department(code, city.dept_name));
            }
            deptMap.get(code).addCity(city);
        });

        // Associer les hôpitaux aux départements
        this.data.individual.hospitals.forEach(hospital => {
            const cityName = this.cityService.findCityName(
                Number(hospital.location.latitude),
                Number(hospital.location.longitude)
            );

            // Trouver le département de cet hôpital
            const cleanName = cityName.replace(' (approx.)', '');
            let hostCity = this.data.cities.find(c => c.name === cleanName);

            if (!hostCity) {
                hostCity = this.cityService.findNearest(
                    Number(hospital.location.latitude),
                    Number(hospital.location.longitude)
                );
            }

            if (hostCity) {
                const dept = deptMap.get(hostCity.dept_code);
                if (dept) {
                    dept.addHospital(cityName, hospital.bed_count);
                }
            }
        });

        return Array.from(deptMap.values())
            .sort((a, b) => String(a.code).localeCompare(String(b.code), 'fr'));
    }

    renderIndividual() {
        const { individual } = this.data;
        const items = [
            `Score : ${FormatService.formatNumber(individual.score)}`,
            `Population isolée : ${FormatService.formatNumber(individual.isolated_population)}`,
            `Nombre de CHRU : ${individual.chru_count}`,
            `Nombre d'hôpitaux : ${individual.hospitals.length}`
        ];
        document.getElementById('individual-list').innerHTML =
            items.map(item => `<li>${item}</li>`).join('');
    }

    renderDepartments() {
        let html = `
            <p class="context-line">Synthèse par département (rayon de couverture : ${GeoService.COVERAGE_RADIUS_KM} km)</p>
        `;

        this.departments.forEach(dept => {
            const desert = dept.getDesertMetrics(this.data.individual.hospitals);
            const hospitalRows = this.buildHospitalRows(dept);

            html += `
                <article class="department-card">
                    <h3>${dept.name} (${dept.code})</h3>

                    <div class="stats-grid">
                        <section class="stat-card">
                            <p class="stat-label">Nombre d'hôpitaux du département</p>
                            <p class="stat-value">${FormatService.formatNumber(dept.hospitals.length)}</p>
                        </section>

                        <section class="stat-card">
                            <p class="stat-label">Désert hospitalier</p>
                            <p class="stat-value">${FormatService.formatNumber(desert.count)} communes</p>
                            <p class="stat-subvalue">${FormatService.formatNumber(desert.population)} habitants</p>
                        </section>

                        <section class="stat-card">
                            <p class="stat-label">Taux de lits / population totale</p>
                            <p class="stat-value">${FormatService.formatBedRate(dept.getBedRate())} lits / 1 000 hab.</p>
                        </section>
                    </div>

                    <h4>Villes accueillant un hôpital (nombre de lits)</h4>
                    <table class="hospital-table">
                        <thead>
                            <tr>
                                <th>Ville</th>
                                <th>Lits</th>
                            </tr>
                        </thead>
                        <tbody>
                            ${hospitalRows}
                        </tbody>
                    </table>
                </article>
            `;
        });

        document.getElementById('stats-content').innerHTML = html;
    }

    buildHospitalRows(dept) {
        if (dept.hospitals.length === 0) {
            return '<tr><td colspan="2">Aucun hôpital dans ce département</td></tr>';
        }

        return dept.hospitals
            .sort((a, b) => b.bedCount - a.bedCount)
            .map(hospital => `
                <tr>
                    <td>${hospital.cityName}</td>
                    <td>${FormatService.formatNumber(hospital.bedCount)}</td>
                </tr>
            `)
            .join('');
    }

    render() {
        this.renderIndividual();
        this.renderDepartments();
    }
}

// ============================================================================
// INITIALIZATION
// ============================================================================

const data = window.reportData;

if (data) {
    const report = new ReportGenerator(data);
    report.render();
}
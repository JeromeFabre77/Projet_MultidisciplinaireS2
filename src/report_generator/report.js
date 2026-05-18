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

    addHospital(cityName, bedCount, latitude, longitude) {
        this.hospitals.push({ cityName, bedCount, latitude, longitude });
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
            // On trouve la ville exacte via les coordonnées de l'hôpital
            const hostCity = this.cityService.findExact(
                Number(hospital.location.latitude),
                Number(hospital.location.longitude)
            );

            // On ajoute directement au département de la ville trouvée
            if (hostCity) {
                const dept = deptMap.get(hostCity.dept_code);
                if (dept) {
                    dept.addHospital(hostCity.name, hospital.bed_count, Number(hospital.location.latitude), Number(hospital.location.longitude));
                }
            } else {
                console.warn(`Hôpital ignoré : aucune commune exacte trouvée aux coordonnées ${hospital.location.latitude}, ${hospital.location.longitude}`);
            }
        });

        return Array.from(deptMap.values())
            .sort((a, b) => String(a.code).localeCompare(String(b.code), 'fr'));
    }

    renderGlobalSummary() {
        const { individual } = this.data;
        const totalPop = this.departments.reduce((sum, dept) => sum + dept.getTotalPopulation(), 0);
        const totalBeds = this.departments.reduce((sum, dept) => sum + dept.getTotalBeds(), 0);
        
        const html = `
            <div class="stats-grid" style="margin-bottom: 12px;">
                <section class="stat-card">
                    <p class="stat-label">Score d'évaluation global</p>
                    <p class="stat-value">${FormatService.formatNumber(individual.score)}</p>
                    <p class="stat-subvalue">Fonction objectif minimisée</p>
                </section>
                <section class="stat-card">
                    <p class="stat-label">Population totale</p>
                    <p class="stat-value">${FormatService.formatNumber(totalPop)}</p>
                    <p class="stat-subvalue">Habitants étudiés</p>
                </section>
                <section class="stat-card">
                    <p class="stat-label">Population isolée non couverte</p>
                    <p class="stat-value" style="color: #ef4444;">${FormatService.formatNumber(individual.isolated_population)}</p>
                    <p class="stat-subvalue">${((individual.isolated_population / totalPop) * 100).toFixed(2)}% du total</p>
                </section>
            </div>
            
            <div class="stats-grid">
                <section class="stat-card">
                    <p class="stat-label">Nombre total d'hôpitaux</p>
                    <p class="stat-value">${FormatService.formatNumber(individual.hospitals.length)}</p>
                    <p class="stat-subvalue">Infrastructures déployées</p>
                </section>
                <section class="stat-card">
                    <p class="stat-label">Centres Hospitaliers (CHRU)</p>
                    <p class="stat-value">${FormatService.formatNumber(individual.chru_count)}</p>
                    <p class="stat-subvalue">Établissements majeurs</p>
                </section>
                <section class="stat-card">
                    <p class="stat-label">Capacité d'accueil globale</p>
                    <p class="stat-value">${FormatService.formatNumber(totalBeds)}</p>
                    <p class="stat-subvalue">${FormatService.formatBedRate((totalBeds / totalPop) * 1000)} lits / 1 000 hab.</p>
                </section>
            </div>
        `;
        document.getElementById('global-stats').innerHTML = html;
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

                    <div id="map-dept-${dept.code}" class="map-container dept-map"></div>

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
        this.renderGlobalSummary();
        this.renderDepartments();
        this.renderMaps();
    }

    renderMaps() {
        // Paramètres de base pour Leaflet visant à désactiver les animations pour Puppeteer
        const mapOptions = {
            zoomControl: false,
            zoomAnimation: false,
            fadeAnimation: false,
            markerZoomAnimation: false
        };

        // --- 1. Carte globale ---
        const globalMap = L.map('global-map', mapOptions).setView([46.603354, 1.888334], 5);
        L.tileLayer('https://{s}.tile.openstreetmap.fr/osmfr/{z}/{x}/{y}.png', {
            attribution: '&copy; OpenStreetMap France | &copy; OpenStreetMap contributors'
        }).addTo(globalMap);

        const globalBounds = [];
        this.data.individual.hospitals.forEach(h => {
            const lat = Number(h.location.latitude);
            const lon = Number(h.location.longitude);
            L.circleMarker([lat, lon], {
                radius: 4,
                color: '#2563eb',
                fillColor: '#3b82f6',
                fillOpacity: 0.7
            }).addTo(globalMap);
            globalBounds.push([lat, lon]);
        });
        
        if (globalBounds.length > 0) {
            globalMap.fitBounds(globalBounds, { padding: [20, 20], animate: false });
        }

        // --- 2. Cartes départementales ---
        this.departments.forEach(dept => {
            if (dept.hospitals.length === 0) return; // Pas de carte s'il n'y a pas d'hôpitaux

            const mapId = `map-dept-${dept.code}`;
            const deptMap = L.map(mapId, mapOptions);
            
            L.tileLayer('https://{s}.tile.openstreetmap.fr/osmfr/{z}/{x}/{y}.png', {
                attribution: '&copy; OpenStreetMap France'
            }).addTo(deptMap);

            const deptBounds = [];
            dept.hospitals.forEach(h => {
                L.circleMarker([h.latitude, h.longitude], {
                    radius: 6,
                    color: '#ef4444',
                    fillColor: '#f87171',
                    fillOpacity: 0.8
                }).addTo(deptMap);
                deptBounds.push([h.latitude, h.longitude]);
            });

            if (deptBounds.length > 0) {
                if (deptBounds.length === 1) {
                    deptMap.setView(deptBounds[0], 10);
                } else {
                    deptMap.fitBounds(deptBounds, { padding: [10, 10], animate: false });
                }
            }
        });
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
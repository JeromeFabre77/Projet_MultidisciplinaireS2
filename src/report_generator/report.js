// Data injected by Puppeteer
const data = window.reportData;

if (data) {
    renderIndividual(data.individual);
    renderDepartmentStats(data);
}

function renderIndividual(ind) {
    const list = document.getElementById('individual-list');
    const items = [
        `Score : ${ind.score}`,
        `Population isolée : ${ind.isolated_population}`,
        `Nombre de CHRU : ${ind.chru_count}`,
        `Nombre d'hôpitaux : ${ind.hospitals.length}`
    ];
    
    list.innerHTML = items.map(item => `<li>${item}</li>`).join('');
}

function renderDepartmentStats(data) {
    const container = document.getElementById('stats-content');
    
    // Calculs pour le département (MVP basé sur les données reçues)
    const totalPopulation = data.cities.reduce((sum, c) => sum + parseInt(c.population), 0);
    const totalBeds = data.individual.hospitals.reduce((sum, h) => sum + h.bed_count, 0);
    const bedRatio = (totalBeds / totalPopulation * 1000).toFixed(2); // Pour 1000 habitants

    let html = `
        <p><strong>Nombre d'hôpitaux :</strong> ${data.individual.hospitals.length}</p>
        <p><strong>Nombre de communes :</strong> ${data.cities.length}</p>
        <p><strong>Population totale :</strong> ${totalPopulation.toLocaleString()}</p>
        <p><strong>Taux de lits :</strong> ${bedRatio} lits pour 1000 hab.</p>
        
        <h3>Villes accueillant un hôpital :</h3>
        <ul>
            ${data.individual.hospitals.map(h => {
                // On cherche la ville correspondante par coordonnées (simplifié pour le MVP)
                const city = data.cities.find(c => c.latitude === h.location.latitude) || { name: "Zone Inconnue" };
                return `<li>${city.name} : ${h.bed_count} lits</li>`;
            }).join('')}
        </ul>
    `;
    
    container.innerHTML = html;
}
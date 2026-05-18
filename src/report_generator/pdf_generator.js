const puppeteer = require('puppeteer');
const path = require('path');
const filesystem = require('fs');


const jsonPathArg = process.argv[2];
if (!jsonPathArg) {
    console.error('Error: No path to the JSON file provided.');
    console.log('Usage: node pdf_generator.js path/to/data.json');
    process.exit(1);
}

const DATA_FILE_PATH = path.resolve(jsonPathArg); // Get the absolute path
if (!filesystem.existsSync(DATA_FILE_PATH)) {
    console.error(`Error: File not found at ${DATA_FILE_PATH}`);
    process.exit(1);
}

const OUTPUT_DIRECTORY = path.join(__dirname, '../../assets/data/result');
if (!filesystem.existsSync(OUTPUT_DIRECTORY)) {
    console.error(`Error: Output direcory not found at ${OUTPUT_DIRECTORY}`);
    process.exit(1);
}

(async () => {
    const rawData = filesystem.readFileSync(DATA_FILE_PATH, 'utf8');
    const jsonData = JSON.parse(rawData);

    // Prepare filename with timestamp
    const now = new Date();
    const timestamp = now.toISOString().replace(/T/, '_').replace(/\..+/, '').replace(/:/g, '-');
    const outputPath = path.join(OUTPUT_DIRECTORY, `report_${timestamp}.pdf`);
    const outputPathLong = path.join(OUTPUT_DIRECTORY, `report_${timestamp}_version_longue.pdf`);

    const browser = await puppeteer.launch();
    const page = await browser.newPage();

    // Inject data into the window object BEFORE the page scripts run
    await page.evaluateOnNewDocument((data) => {
        window.reportData = data;
    }, jsonData);

    const filePath = `file://${path.join(__dirname, 'report.html')}`;
    await page.goto(filePath, { waitUntil: 'networkidle0' });

    // Génération du 1er PDF (Format A4 classique)
    await page.pdf({
        path: outputPath,
        format: 'A4',
        printBackground: true, // To keep the CSS style
        margin: { top: '1cm', right: '1cm', bottom: '1cm', left: '1cm' }
    });
    console.log(`Report A4 successfully generated: ${outputPath}`);

    // Préparation pour le 2ème PDF (Version longue en une seule page)
    await page.evaluate(() => {
        const style = document.createElement('style');
        style.innerHTML = `
            @media print {
                .page-break { page-break-before: avoid !important; display: none !important; }
                .cover-page { min-height: auto !important; margin-bottom: 2cm !important; }
            }
        `;
        document.head.appendChild(style);
    });

    // Récupération de la hauteur dynamique de la page html
    const scrollHeight = await page.evaluate(() => document.documentElement.offsetHeight);

    await page.pdf({
        path: outputPathLong,
        width: '210mm', // On conserve la largeur d'une page A4 pour ne pas casser le style
        height: `${scrollHeight + 150}px`, // Hauteur totale + marge de sécurité
        printBackground: true,
        margin: { top: '1cm', right: '1cm', bottom: '1cm', left: '1cm' }
    });

    console.log(`Report version longue successfully generated: ${outputPathLong}`);
    await browser.close();
})();
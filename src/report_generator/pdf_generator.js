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

const OUTPUT_DIRECTORY = path.join(__dirname, '../../assets/results');
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

    const browser = await puppeteer.launch();
    const page = await browser.newPage();

    // Inject data into the window object BEFORE the page scripts run
    await page.evaluateOnNewDocument((data) => {
        window.reportData = data;
    }, jsonData);

    const filePath = `file://${path.join(__dirname, 'report.html')}`;
    await page.goto(filePath, { waitUntil: 'networkidle0' });

    await page.pdf({
        path: outputPath,
        format: 'A4',
        printBackground: true, // To keep the CSS style
        margin: { top: '1cm', right: '1cm', bottom: '1cm', left: '1cm' }
    });

    console.log(`Report successfully generated: ${outputPath}`);
    await browser.close();
})();
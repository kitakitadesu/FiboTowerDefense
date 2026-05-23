const logs = [];
page.on('console', msg => logs.push(msg.type() + ': ' + msg.text()));
page.on('pageerror', err => logs.push('ERROR: ' + err.message));
try {
  await page.goto('https://fibotowerdefense-9feb08.p.bocchichan.moe/', { timeout: 15000, waitUntil: 'domcontentloaded' });
  await new Promise(r => setTimeout(r, 5000));
} catch(e) {
  logs.push('NAV_ERROR: ' + e.message);
}
const title = await page.title();
const url = page.url();
// Print result
console.log('TITLE: ' + title);
console.log('URL: ' + url);
console.log('LOGS: ' + JSON.stringify(logs));

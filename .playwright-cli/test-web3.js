var logs = [];
page.on('console', function(msg) { logs.push(msg.type() + ': ' + msg.text()); });
page.on('pageerror', function(err) { logs.push('ERROR: ' + err.message); });
try {
  await page.goto('https://fibotowerdefense-9feb08.p.bocchichan.moe/', { timeout: 15000, waitUntil: 'domcontentloaded' });
  await new Promise(function(r) { setTimeout(r, 5000); });
} catch(e) {
  logs.push('NAV_ERROR: ' + e.message);
}
var title = await page.title();
var url = page.url();
console.log('TITLE: ' + title);
console.log('URL: ' + url);
console.log('LOGS: ' + JSON.stringify(logs));

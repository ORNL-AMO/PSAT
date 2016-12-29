Electron = require('electron');
App = Electron.app;

App.on('ready', function() {
	let arg = {width: 1280, height: 1024};
	if (process.platform == 'linux') {
		arg.icon = __dirname + '/pump.png';
	}
	Win = new Electron.BrowserWindow(arg);
	Win.loadURL(`file://${__dirname}/index.html`);
})

App.on('window-all-closed', function () {
		App.quit();
})


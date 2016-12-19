Electron = require('electron');
App = Electron.app;

App.on('ready', function() {
	Win = new Electron.BrowserWindow({width: 1280, height: 1024});
	Win.loadURL(`file://${__dirname}/index.html`);
})

App.on('window-all-closed', function () {
		App.quit();
})


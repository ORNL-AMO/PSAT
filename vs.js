Electron = require('electron')
App = Electron.app


App.on('ready', function() {
  Win = new Electron.BrowserWindow();
  Win.loadURL(`file://${__dirname}/vs.html`);
})


App.on('window-all-closed', function () {
    App.quit();
})


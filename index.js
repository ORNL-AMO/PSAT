Electron = require('electron')
App = Electron.app

//rm if prod
require('electron-reload')(__dirname);

App.on('ready', function() {
  Win = new Electron.BrowserWindow({width: 800, height: 600});
  Win.loadURL(`file://${__dirname}/index.html`);
})

App.on('window-all-closed', function () {
    App.quit();
})

// const bridge = require('./build/Release/bridge');
// console.log(bridge.pumpEff());

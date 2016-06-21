Electron = require('electron')
App = Electron.app

//rm if prod
// require('electron-reload')(__dirname);

// App.on('ready', function() {
//   Win = new Electron.BrowserWindow({width: 1280, height: 1024});
//   Win.loadURL(`file://${__dirname}/test.html`);
// })

// App.on('window-all-closed', function () {
//     App.quit();
// })


var ffi = require('ffi');

var libm = ffi.Library('libm', {
'ceil': [ 'double', [ 'double' ] ]
});

console.log('hi');
//console.log(libm.ceil(1.5));

// var api = ffi.Library('/Users/Worky/Documents/Dev/psat/repo/app/libpsat_engine.dylib', {
//         'aTest': ['double', []]
// });

// var api = ffi.Library('libhello', {
// 	'hello': ['int',[]]
// });
// console.log(api.hello());


var api = ffi.Library('libhello4', {
	'dummy': ['int',[]]
});
console.log(api.dummy());
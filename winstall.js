var electronInstaller = require('electron-winstaller');

resultPromise = electronInstaller.createWindowsInstaller({
    appDirectory: '../releases/PSAT-win32-x64',
    outputDirectory: '../winstall',
    authors: 'ORNL-AMO',
    exe: 'psat.exe'
  });

resultPromise.then(() => console.log("It worked!"), (e) => console.log(`No dice: ${e.message}`));
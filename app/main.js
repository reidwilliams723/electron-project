/*
// Modules to control application life and create native browser window
const {app, BrowserWindow} = require('electron')

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

function createWindow () {

  // Create the browser window.
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true
    }
  })

  //mainWindow.setFullScreen(true);
  // and load the index.html of the app.
  mainWindow.loadFile('app/index.html')

  // Open the DevTools.
  // mainWindow.webContents.openDevTools()

  // Emitted when the window is closed.
  mainWindow.on('closed', function () {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    mainWindow = null
  })
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow)

// Quit when all windows are closed.
app.on('window-all-closed', function () {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', function () {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (mainWindow === null) {
    createWindow()
  }
})*/

const { app, BrowserWindow} = require('electron');

const windows = new Set();

app.on('ready', () => {
  createWindow('index.html', 800, 600, true);
});

app.on('window-all-closed', () => {
  if (process.platform === 'darwin') {
    return false;
  }
});

app.on('activate', (event, hasVisibleWindows) => {
  if (!hasVisibleWindows) { createWindow('index.html', 800, 600, true); }
});

const createWindow = exports.createWindow = (page, width, height, enableFrame) => {
  

  let x, y;
  const currentWindow = BrowserWindow.getFocusedWindow();
  if (currentWindow) {
    x = currentWindow.width / 2;
    y = currentWindow.height / 2;
  }
  let newWindow = new BrowserWindow({ x, y, width: width, height: height, show: false, frame: enableFrame });
  newWindow.loadURL(`file://${__dirname}/${page}`);
  newWindow.webContents.openDevTools()
  newWindow.once('ready-to-show', () => {
    newWindow.show();
});

newWindow.on('closed', () => {
    windows.delete(newWindow);
    newWindow = null;
});
  windows.add(newWindow); 
  return newWindow;
};

// Modules to control application life and create native browser window
const {app, BrowserWindow, ipcMain} = require('electron')
const ProgressBar = require('electron-progressbar');
// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow
let progressBar;
let uploadStatus;

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
  mainWindow.loadFile('./app/index.html')

  ipcMain.on('show-progressbar', showProgressbar);
	
  ipcMain.on('set-progressbar-completed', (event, args) => setProgressbarCompleted(args));
  
  ipcMain.on('update-progressbar', (event, args) => updateProgressText(args))

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
})

function showProgressbar () {
	if (progressBar) {
		return;
	}
	
	progressBar = new ProgressBar({
		text: 'Uploading...',
    detail: 'Wait...',
    closeOnComplete: true,
		browserWindow: {
      parent: mainWindow,
      modal: true,
      closable: true
		}
	});
	
	progressBar
		.on('completed', function() {
			progressBar = null;
    });
    
    progressBar
      .on('aborted', function() {
        progressBar = null;
      });
	
	// launch the task...
	// launchTask();
}

function setProgressbarCompleted (args) {
	if (progressBar) {
    uploadStatus = 'Success.'
    progressBar.text = 'Upload ' + args
    progressBar.detail = 'Exiting...';
    setTimeout(function(){
      progressBar.setCompleted();
    }, 3000);
	}
}

function updateProgressText(args) {
  if (progressBar) {
    progressBar.text = args;
    setTimeout(function(){
    }, 3000);
  }
}
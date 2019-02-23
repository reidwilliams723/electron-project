const { remote, ipcRenderer } = require('electron');
const ipc = require('electron').ipcRenderer;
const mainProcess = remote.require('./main.js');

var btns = document.querySelectorAll('.btn');

btns.forEach(function(btn) {
    if(!btn.classList.contains('flash') && !btn.classList.contains('test')) {
        btn.addEventListener('click', (btn) => {
        var newSectionName = btn.srcElement.getAttribute('name');

        var sectionId = getSectionId(btn);
        document.getElementById(sectionId).hidden = true;
        document.getElementById(newSectionName).hidden = false;
        })
    }
});

function getSectionId(btn){
    btn = btn.srcElement;
    if(btn.getAttribute('class').includes('back')){
        return btn.parentElement.parentElement.parentElement.parentElement.id;
    }
    else
        return btn.parentElement.parentElement.parentElement.id;
}

var flashBtns = document.querySelectorAll('.flash');

flashBtns.forEach(function(btn) {btn.addEventListener('click', (btn) => {
    
    ipc.send('show-progressbar');
        
    var folder = '';
    var board = btn.srcElement.getAttribute('name');
    var port = '/dev/cu.usbmodem142230';

    switch (board){
        case 'analog':
            folder = 'AnalogInputs4_20';
            break;
        case 'frequency':
            folder += 'FreqInput_Type5';
            break;
        case 'canbus':
            folder += 'Engine_Can_Type_7';
            break;
        case 'mosfet':
            folder += 'MOSFETOut';
            break;
    }

    ipc.send('update-progressbar', "Burning Bootloader...");
    runBootLoader(folder);
})});

function runBootLoader(folder){
    var bootLoader = require('child_process').spawn('bash', ['./app/scripts/bootloader.sh']);
    bootLoader.stdout.pipe(process.stdout);
    bootLoader.stderr.pipe(process.stderr);

    bootLoader.on('exit', function(code){
        if(code === 0){
            ipc.send('update-progressbar', "Bootloader complete!");
            runfirmwareInstallation(folder)
        }
        else if (data.includes('Upload Failed')){
            ipc.send('set-progressbar-completed', "Upload Failed");
        }
    });
    
}

function runfirmwareInstallation(folder){
    ipc.send('update-progressbar', "Uploading Firmware...");

    var python = require('child_process').spawn('bash', ['./app/scripts/firmwareinstall.sh', folder]);

    python.stdout.pipe(process.stdout);
    python.stderr.pipe(process.stderr);

    python.on('exit', function(code){
        if(code === 0){
            ipc.send('set-progressbar-completed', "Upload Success");
        }
        else {
            ipc.send('set-progressbar-completed', "Upload Failed");
        }
    })
}

function runTest(card_type){

    var python = require('child_process').spawn('python3', ['./app/scripts/test.py', card_type]);

    python.stdout.pipe(process.stdout);
    python.stderr.pipe(process.stderr);

    python.on('exit', function(code){
        if(code === 0){
            ipc.send('set-progressbar-completed', "Test Successful");
        }
        else {
            ipc.send('set-progressbar-completed', "Test Failed");
        }
    })
}

var testBtns = document.querySelectorAll('.test');

testBtns.forEach(function(btn) {btn.addEventListener('click', (btn) => {
    
    ipc.send('show-progressbar');
        
    var card_type = 0;
    var board = btn.srcElement.getAttribute('name');

    switch (board){
        case 'analog':
            card_type = 1;
            break;
        case 'frequency':
            card_type = 2;
            break;
        case 'canbus':
            card_type = 7; 
            break;
        case 'mosfet':
            card_type = 4;
            break;
    }

    ipc.send('update-progressbar', "Running Test...");
    runTest(card_type);
})});

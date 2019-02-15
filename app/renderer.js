const { remote, ipcRenderer } = require('electron');
const mainProcess = remote.require('./main.js');

var btns = document.querySelectorAll('.btn');

btns.forEach(function(btn) {
    if(!btn.classList.contains('flash')) {
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
    if(btn.parentElement.getAttribute('class') === 'container'){
        return btn.parentElement.parentElement.parentElement.id;
    }
    else
        return btn.parentElement.parentElement.parentElement.parentElement.parentElement.id;
}

var flashBtns = document.querySelectorAll('.flash');

flashBtns.forEach(function(btn) {btn.addEventListener('click', (btn) => {
    const ipc = require('electron').ipcRenderer;
    
    ipc.send('show-progressbar');
        
    var folder = '';
    var board = btn.srcElement.getAttribute('name');
    var port = '/dev/cu.usbmodem141240';

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
    
    var python = require('child_process').spawn('bash', ['./app/scripts/firmwareupdates.sh', folder]);
    python.stdout.pipe(process.stdout);
    python.stderr.pipe(process.stderr);
    python.stdout.setEncoding('utf8');
    python.stdout.on('data', function(data){
        if(data.includes('Upload Success')){
            ipc.send('set-progressbar-completed');
        }
        else if (data.includes('Upload Failed')){
            ipc.send('progressbar-failed');
        }
    })
})});

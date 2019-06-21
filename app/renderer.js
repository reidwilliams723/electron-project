const { remote, ipcRenderer } = require('electron');
const ipc = require('electron').ipcRenderer;
const mainProcess = remote.require('./main.js');
var btns = document.querySelectorAll('.btn');
let test;

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
        return btn.parentElement.parentElement.id;
    }
    else
        return btn.parentElement.parentElement.parentElement.id;
}

var flashBtnsSBC = document.querySelectorAll('.flashSBC')

flashBtnsSBC.forEach(function(btn) {btn.addEventListener('click', (btn) => {
    flashSBC();
})});

function flashSBC() {
    var flash =require('child_process').spawn('bash',['./app/scripts/sbc-flash.sh']);
}

var flashBtns = document.querySelectorAll('.flash');

flashBtns.forEach(function(btn) {btn.addEventListener('click', (btn) => {
     
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

    ipc.send('open-modal', {modalType: "Uploading", folder: folder});

})});



var testBtns = document.querySelectorAll('.test');

testBtns.forEach(function(btn) {btn.addEventListener('click', (btn) => { 
    let card_type;
    let board = btn.srcElement.getAttribute('name');

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

    ipc.send('open-modal', {modalType: "Testing", cardType: card_type});
})});


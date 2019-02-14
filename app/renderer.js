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
    mainProcess.createWindow('status-popup.html', 400, 400, false);
    //var python = require('child_process').spawn('python', ['./app/python/hello.py']);
    //var python = require('child_process').spawn('./app/arduino/arduino-cli', ['version']);
    //python.stdout.on('data',function(data){
    //    console.log("data: ",data.toString('utf8'));
    //});
})});
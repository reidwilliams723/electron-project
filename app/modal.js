const { remote, ipcRenderer } = require('electron');
const ipc = require('electron').ipcRenderer;

let spawn;

ipc.on('get-data', function(event, args){

    document.querySelector('#header').innerHTML = args.modalType;
    document.title= args.modalType;
    switch(args.modalType){
        case "Testing":
            runTest(args.cardType);
            break;
        case "Uploading":
            runBootLoader(args.folder);
            //runfirmwareInstallation(args.folder);
            break;
    } 
});


document.querySelector('#closeBtn').addEventListener('click', () => {
    if(spawn.exitCode == null)
        process.kill(spawn.pid);

    modal = remote.getCurrentWindow();
    modal.close();

});

function runTest(card_type){

    spawn = require('child_process').spawn('python3', ['-u', './app/scripts/test.py', card_type]);

    spawn.stdout.pipe(process.stdout);
    spawn.stderr.pipe(process.stderr);

    spawn.stdout.on('data', function(data){
        data = data.toString();
        var mydiv = document.createElement('div');
        mydiv.innerHTML = data;
        console.log("HI");
        document.querySelector('#data').append(mydiv);
    });

    spawn.stderr.on('data', function(data){
        data = data.toString();

        var mydiv = document.createElement('div');
        mydiv.innerHTML = data;
        document.querySelector('#data').append(mydiv);
    });
}

function runfirmwareInstallation(folder){
    spawn = require('child_process').spawn('bash', ['./app/scripts/firmwareinstall.sh', folder]);

    spawn.stdout.pipe(process.stdout);
    spawn.stderr.pipe(process.stderr);

    spawn.stdout.on('data', function(data){
        data = data.toString();
        var mydiv = document.createElement('div');
        mydiv.innerHTML = data;
        document.querySelector('#data').appendChild(mydiv);
    });

    spawn.stderr.on('data', function(data){
        data = data.toString();
        var mydiv = document.createElement('div');
        mydiv.innerHTML = data;
        document.querySelector('#data').appendChild(mydiv);
    });
}

function runBootLoader(folder){
    spawn = require('child_process').spawn('bash', ['./app/scripts/bootloader.sh']);
    spawn.stdout.pipe(process.stdout);
    //bootLoader.stderr.pipe(process.stderr);

    spawn.stdout.on('data', function(data){

        data = data.toString();
        var mydiv = document.createElement('div');
        mydiv.innerHTML = data;
        document.querySelector('#data').append(mydiv);
        document.querySelector('#data').appendChild(document.createElement('br'));
    });

    spawn.stderr.on('data', function(data){
        data = data.toString();
        var mydiv = document.createElement('div');
        mydiv.innerHTML = data;
        document.querySelector('#data').appendChild(mydiv);
    });
    
}

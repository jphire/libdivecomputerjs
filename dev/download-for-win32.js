const http = require('http');
const https = require('https');
const URL = require('url');
const fs = require('fs');
const path = require('path');

async function downloadFile(url, dest) {
    const urlParts = new URL.URL(url);
    const filename = path.basename(urlParts.pathname);
    
    let httpModule = http;
    if (urlParts.protocol.includes('https')) {
        httpModule = https;
    }

    return new Promise(function(resolve, reject) {
        const file = fs.createWriteStream(path.join(dest, filename));
        var request = httpModule.get(url, function(response) {
            response.pipe(file);
            file.on('finish', function() {
                file.close();
                resolve();
            });
        }).on('error', function(err) {
            fs.unlink(dest);
            reject(err);
        });

    });
}

async function ensureDirectory(dir) {
    return fs.promises.mkdir(dir, { recursive: true });
}

const dllSource = 'https://www.libdivecomputer.org/builds/divinglog/';
const dllFiles = [
    'libdivecomputer-0.dll',
    'libhidapi-0.dll',
    'libusb-1.0.dll',
];

// const headerFileSource = 'https://raw.githubusercontent.com/libdivecomputer/libdivecomputer/master/include/libdivecomputer/';
// const headerFiles = [
//     'atomics_cobalt.h',
//     'ble.h',
//     'bluetooth.h',
//     'buffer.h',
//     'common.h',
//     'context.h',
//     'custom.h',
//     'datetime.h',
//     'descriptor.h',
//     'device.h',
//     'divesystem_idive.h',
//     'hw_frog.h',
//     'hw_ostc.h',
//     'hw_ostc3.h',
//     'ioctl.h',
//     'iostream.h',
//     'irda.h',
//     'iterator.h',
//     'oceanic_atom2.h',
//     'oceanic_veo250.h',
//     'oceanic_vtpro.h',
//     'parser.h',
//     'reefnet_sensus.h',
//     'reefnet_sensuspro.h',
//     'reefnet_sensusultra.h',
//     'serial.h',
//     'suunto_d9.h',
//     'suunto_eon.h',
//     'suunto_vyper2.h',
//     'units.h',
//     'usb.h',
//     'usbhid.h',
//     'usb.h',

// ];

async function main() {
    const dest = process.argv[2];
    const libDir = path.join(dest, 'lib');
    
    await ensureDirectory(dest);
    for(const file of dllFiles) {
        await downloadFile(dllSource + file, libDir);
    }    
}

main();

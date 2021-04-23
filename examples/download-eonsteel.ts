const SegfaultHandler = require('segfault-handler');
SegfaultHandler.registerHandler('crash.log');
import {
    AsyncDeviceReader,
    Context,
    EventType,
    LogLevel,
    test,
    USBHIDTransport,
    version,
} from 'libdivecomputerjs';
import { getEonSteel } from './helpers/descriptors';
import fs from 'fs';

const OUTPUTS_DIR = './outputs';
if (!fs.existsSync(OUTPUTS_DIR)) {
    fs.mkdirSync(OUTPUTS_DIR);
}

console.log(`libdivecomputer v${version()}`);

const context = new Context();
context.onLog((lvl, msg) => {
    console.log(`[CTX1][Log][${lvl}]: ${msg}`);
});
context.logLevel = LogLevel.Warning;

const eonSteel = getEonSteel();

if (eonSteel === undefined) {
    throw new Error('No Eonsteel descriptor found');
}

const usbhid = Array.from(USBHIDTransport.iterate(context, eonSteel))[0];

if (usbhid === undefined) {
    throw new Error('No EonSteel USBHID device found');
}

console.log(`Found device ${usbhid.toString()}`);

const runner = new AsyncDeviceReader();
runner.setContext(context);
runner.setDescriptor(eonSteel);
runner.setTransport(usbhid);

runner.onEvents([EventType.Progress], console.log);
runner.onDive((dive, fingerprint) =>
    console.log(fingerprint.toString('base64'))
);

runner.read((err) => console.log('Done', err));

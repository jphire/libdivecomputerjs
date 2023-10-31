import {
    AsyncDeviceReader,
    Context,
    EventType,
    LogLevel,
    USBHIDTransport,
    version,
} from 'libdivecomputerjs';
import { getEonSteel } from './helpers/descriptors';
import fs from 'react-native-fs';

const OUTPUTS_DIR = './outputs';
if (!fs.existsSync(OUTPUTS_DIR)) {
    fs.mkdirSync(OUTPUTS_DIR);
}

console.log(`libdivecomputer v${version()}`);

const context = new Context();
context.onLog((lvl: number, msg: string) => {
    console.log(`[CTX][Log][${lvl}]: ${msg}`);
});
context.logLevel = LogLevel.Warning;

const eonSteel = getEonSteel();

if (eonSteel === undefined) {
    throw new Error('No Eonsteel descriptor found');
}

const usbhid: USBHIDTransport = Array.from(USBHIDTransport.iterate(context, eonSteel))[0];

if (usbhid === undefined) {
    throw new Error('No EonSteel USBHID device found');
}

console.log(`Found device ${usbhid.toString()}`);

const runner = new AsyncDeviceReader();
runner.setContext(context);
runner.setDescriptor(eonSteel);
runner.setTransport(usbhid);

runner.onEvents([EventType.Progress, EventType.DevInfo], (args) => {
    switch (args.type) {
        case EventType.Progress:
            console.log(args);
            break;
        case EventType.DevInfo:
            console.log('device: ', args.data.serial);
            runner.setFingerprint(Buffer.from('E6RbXw==', 'base64'));
            break;
    }
});
runner.onDive((dive, fingerprint) =>
    console.log(fingerprint.toString('base64'))
);
runner.onDevice((device) => console.log(device));

runner.read((err) => console.log('Done', err));

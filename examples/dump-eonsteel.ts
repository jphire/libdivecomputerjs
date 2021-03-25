const SegfaultHandler = require('segfault-handler');
SegfaultHandler.registerHandler('crash.log');
import {
    Context,
    Device,
    EventType,
    LogLevel,
    USBHIDTransport,
    version,
} from 'libdivecomputer';
import { getEonSteel } from './helpers/descriptors';
import fs from 'fs';
import { bundleDiveData } from './helpers/parser';

const DUMPS_DIR = './dumps';
if (!fs.existsSync(DUMPS_DIR)) {
    fs.mkdirSync(DUMPS_DIR);
}

console.log(`libdivecomputer v${version()}`);

const context = new Context();
context.onLog((lvl, msg) => {
    console.log(`[Log][${lvl}]: ${msg}`);
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

const iostream = usbhid.open(context);

let deviceClock = {
    devtime: 0,
    systime: 0n,
};

const device = new Device(context, eonSteel, iostream);
const allEvents = [
    EventType.Clock,
    EventType.DevInfo,
    EventType.Progress,
    EventType.Vendor,
    EventType.Waiting,
];
device.setEvents(allEvents, (event) => {
    console.log(event);

    if (event.type === EventType.Clock) {
        deviceClock.devtime = event.data.devtime;
        deviceClock.systime = event.data.systime;
    }
});

let iX = 0;
device.foreach((diveData: Buffer, fingerprint: Buffer) => {
    const filename = `${eonSteel.vendor}_${eonSteel.product}_${iX++}`;

    const bundledData = bundleDiveData(
        diveData,
        deviceClock.devtime,
        deviceClock.systime
    );

    fs.writeFileSync(`${DUMPS_DIR}/${filename}.bin`, bundledData);
});

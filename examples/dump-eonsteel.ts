import {
    Context,
    Device,
    EventType,
    LogLevel,
    USBHIDTransport,
    version,
} from 'libdivecomputerjs';
import { getEonSteel } from './helpers/descriptors';
import fs from 'react-native-fs';
import { bundleDiveData } from './helpers/parser';
import { Buffer } from 'buffer/';

const DUMPS_DIR = './dumps';
if (!fs.existsSync(DUMPS_DIR)) {
    fs.mkdirSync(DUMPS_DIR);
}

console.log(`libdivecomputer v${version()}`);

const context = new Context();
context.onLog((lvl: number, msg: string) => {
    console.log(`[Log][${lvl}]: ${msg}`);
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

console.log(`Found device ${usbhid?.toString()}`);

const iostream = usbhid ? usbhid.open(context) : undefined;

let deviceClock: { devtime: number, systime: bigint} = {
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
device.setEvents(allEvents, (event: EventType) => {
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
        deviceClock.systime.valueOf()
    );

    fs.writeFileSync(`${DUMPS_DIR}/${filename}.bin`, bundledData);
});

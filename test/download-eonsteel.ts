import {
    Context,
    Device,
    EventType,
    FieldType,
    LogLevel,
    Parser,
    SampleType,
    USBHIDTransport,
    version,
} from 'libdivecomputer';
import { getEonSteel } from './helpers/descriptors';

console.log(`libdivecomputer v${version()}`);

const context = new Context();
context.onLog((lvl, msg) => {
    console.log(`[Log][${lvl}]: ${msg}`);
});
context.logLevel = LogLevel.Info;
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
});

device.foreach((diveData: ArrayBuffer, fingerprint: ArrayBuffer) => {
    console.log(fingerprint);

    const parser = new Parser();
    parser.setData(diveData);

    console.log('time: ' + parser.getField(FieldType.DiveTime));
    console.log('depth: ' + parser.getField(FieldType.MaxDepth));
    console.log('tank: ', parser.getField(FieldType.Tank));
    console.log('maxTemp: ' + parser.getField(FieldType.TemperatureMaximum));
    console.log('minTemp: ' + parser.getField(FieldType.TemperatureMinimum));

    parser.samplesForeach((sample) => {
        console.log(sample);
    });
});

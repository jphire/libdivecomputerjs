const SegfaultHandler = require('segfault-handler');
SegfaultHandler.registerHandler('crash.log');
import {
    Context,
    Device,
    EventType,
    FieldType,
    LogLevel,
    Parser,
    Sample,
    USBHIDTransport,
    version,
} from 'libdivecomputer';
import { getEonSteel } from './helpers/descriptors';
import fs from 'fs';

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

device.setCancel(() => true);

device.foreach((diveData: Buffer, fingerprint: Buffer) => {
    const parser = new Parser(device);
    parser.setData(diveData);
    const dive = {
        fingerprint: fingerprint.toString('base64'),
        divetime: parser.getField(FieldType.DiveTime),
        maxDepth: parser.getField(FieldType.MaxDepth),
        avgDepth: parser.getField(FieldType.AverageDepth),
        tank: parser.getField(FieldType.Tank),
        tankCount: parser.getField(FieldType.TankCount),
        maxTemp: parser.getField(FieldType.TemperatureMaximum),
        minTemp: parser.getField(FieldType.TemperatureMinimum),
        surfaceTemp: parser.getField(FieldType.TemperatureSurface),
        divemode: parser.getField(FieldType.DiveMode),
        gasmix: parser.getField(FieldType.GasMix),
        salinity: parser.getField(FieldType.Salinity),
        atmospheric: parser.getField(FieldType.Atmospheric),
        samples: [] as Sample[],
    };
    console.log(`Dive ${dive.fingerprint}`);

    console.log('time: ' + parser.getField(FieldType.DiveTime));
    console.log('depth: ' + parser.getField(FieldType.MaxDepth));
    console.log('tank: ', parser.getField(FieldType.Tank));
    console.log('maxTemp: ' + parser.getField(FieldType.TemperatureMaximum));
    console.log('minTemp: ' + parser.getField(FieldType.TemperatureMinimum));

    parser.samplesForeach((sample) => {
        dive.samples.push(sample);
    });

    const asstring = JSON.stringify(dive, null, 4);
    fs.writeFileSync(`./outp/${dive.fingerprint}.json`, asstring);
});

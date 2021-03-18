import { Context, Device, EventType } from 'libdivecomputer';
import { getEonSteel } from './helpers/descriptors';

const context = new Context();
context.onLog((lvl, msg) => {
    console.log('[' + lvl + ']: ' + msg);
});
const eonSteel = getEonSteel();

// const device = new Device();

// device.setEvents([], (args) => {});

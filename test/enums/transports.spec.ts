import { expect } from 'chai';
import libdivecomputer from 'libdivecomputerjs';

describe('Transport', () => {
    it('Has all Transports', () => {
        expect(libdivecomputer.Transport.None).to.equals('None');
        expect(libdivecomputer.Transport.Serial).to.equals('Serial');
        expect(libdivecomputer.Transport.IRDA).to.equals('IRDA');
        expect(libdivecomputer.Transport.USB).to.equals('USB');
        expect(libdivecomputer.Transport.USBHID).to.equals('USBHID');
        expect(libdivecomputer.Transport.Bluetooth).to.equals('Bluetooth');
        expect(libdivecomputer.Transport.BLE).to.equals('BLE');
    });
});

import chai, { expect } from 'chai';
import { Descriptor, Transport } from 'libdivecomputer';
import assertType from 'chai-asserttype';
import { getEonSteel } from './helpers/descriptors';
chai.use(assertType);

describe('descriptors', () => {
    it('lists descriptors', () => {
        const iterator = Descriptor.iterate();
        const array = Array.from(iterator);

        expect(array).to.not.equal(0);
    });

    it('contains descriptors', () => {
        function assertDescriptor(descriptor: Descriptor) {
            expect(descriptor.product).to.be.string();
            expect(descriptor.vendor).to.be.string();
            expect(descriptor.model).to.be.number();
        }

        const iterator = Descriptor.iterate();

        for (const descriptor of iterator) {
            assertDescriptor(descriptor);
        }
    });

    it('contains EON Steel', () => {
        const iterator = Descriptor.iterate();

        expect(
            Array.from(iterator).filter(
                (descriptor: Descriptor) =>
                    descriptor.vendor === 'Suunto' &&
                    descriptor.product === 'EON Steel'
            )
        ).to.have.lengthOf(1, "Didn't find Suunto EON Steel");
    });

    it('cannot create instance of descriptors', () => {
        const iterator = Descriptor.iterate();
        const descriptor = Array.from(iterator)[0];
        expect(
            () => new (Object.getPrototypeOf(descriptor).constructor)()
        ).to.throw();
    });

    it('retrieves transports', () => {
        const eonSteel = getEonSteel();

        const transports = eonSteel.transports;
        expect(transports).to.includes(Transport.USBHID);
    });
});

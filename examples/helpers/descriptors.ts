import { Descriptor } from 'libdivecomputerjs';

export function getEonSteel(): undefined | Descriptor {
    return Array.from(Descriptor.iterate()).filter(
        (descriptor: Descriptor) =>
            descriptor.vendor === 'Suunto' && descriptor.product === 'EON Steel'
    )[0];
}

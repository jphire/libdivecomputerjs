import { Descriptor } from 'libdivecomputer';

export function getEonSteel(): undefined | Descriptor {
    return Array.from(Descriptor.iterate()).filter(
        (descriptor: Descriptor) =>
            descriptor.vendor === 'Suunto' && descriptor.product === 'EON Steel'
    )[0];
}

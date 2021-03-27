import { expect } from 'chai';
import libdivecomputer from 'libdivecomputerjs';

describe('version', () => {
    it('reads current version', () => {
        const value = libdivecomputer.version();
        expect(value).to.equals('0.7.0');
    });
});

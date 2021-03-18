import { expect } from 'chai';
import libdivecomputer from 'libdivecomputer';

describe('version', () => {
    it('reads current version', () => {
        const value = libdivecomputer.version();
        expect(value).to.equals('v0.7.0');
    });
});
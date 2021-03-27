import { expect } from 'chai';
import libdivecomputer from 'libdivecomputerjs';

describe('LogLevels', () => {
    it('Has all LogLevels', () => {
        expect(libdivecomputer.LogLevel.None).to.equals('None');
        expect(libdivecomputer.LogLevel.Error).to.equals('Error');
        expect(libdivecomputer.LogLevel.Warning).to.equals('Warning');
        expect(libdivecomputer.LogLevel.Info).to.equals('Info');
        expect(libdivecomputer.LogLevel.Debug).to.equals('Debug');
        expect(libdivecomputer.LogLevel.All).to.equals('All');
    });
});

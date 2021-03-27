import { expect } from 'chai';
import { EventType } from 'libdivecomputerjs';

describe('EventTypes', () => {
    it('Has all EventTypes', () => {
        expect(EventType.Clock).to.equals('Clock');
        expect(EventType.Waiting).to.equals('Waiting');
        expect(EventType.DevInfo).to.equals('DevInfo');
        expect(EventType.Vendor).to.equals('Vendor');
        expect(EventType.Progress).to.equals('Progress');
    });
});

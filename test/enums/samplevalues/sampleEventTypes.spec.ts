import { expect } from 'chai';
import { SampleEventType } from 'libdivecomputerjs';

describe('SampleEventTypes', () => {
    it('Has all sample event types', () => {
        expect(SampleEventType.None).to.equals('None');
        expect(SampleEventType.Deco).to.equals('Deco');
        expect(SampleEventType.RBT).to.equals('RBT');
        expect(SampleEventType.Ascent).to.equals('Ascent');
        expect(SampleEventType.Ceiling).to.equals('Ceiling');
        expect(SampleEventType.Workload).to.equals('Workload');
        expect(SampleEventType.Transmitter).to.equals('Transmitter');
        expect(SampleEventType.Violation).to.equals('Violation');
        expect(SampleEventType.Bookmark).to.equals('Bookmark');
        expect(SampleEventType.Surface).to.equals('Surface');
        expect(SampleEventType.SafetyStop).to.equals('SafetyStop');
        expect(SampleEventType.Gaschange).to.equals('Gaschange');
        expect(SampleEventType.SafetyStopVoluntary).to.equals(
            'SafetyStopVoluntary'
        );
        expect(SampleEventType.SafetyStopMandatory).to.equals(
            'SafetyStopMandatory'
        );
        expect(SampleEventType.Deepstop).to.equals('Deepstop');
        expect(SampleEventType.CeilingSafetyStop).to.equals(
            'CeilingSafetyStop'
        );
        expect(SampleEventType.Floor).to.equals('Floor');
        expect(SampleEventType.Divetime).to.equals('Divetime');
        expect(SampleEventType.Maxdepth).to.equals('Maxdepth');
        expect(SampleEventType.OLF).to.equals('OLF');
        expect(SampleEventType.PO2).to.equals('PO2');
        expect(SampleEventType.Airtime).to.equals('Airtime');
        expect(SampleEventType.RGBM).to.equals('RGBM');
        expect(SampleEventType.Heading).to.equals('Heading');
        expect(SampleEventType.TissueLevelWarning).to.equals(
            'TissueLevelWarning'
        );
        expect(SampleEventType.Gaschange2).to.equals('Gaschange2');
    });
});

import { expect } from 'chai';
import { SampleType } from 'libdivecomputer';

describe('SampleTypes', () => {
    it('Has all samples types', () => {
        expect(SampleType.Bearing).to.equals('Bearing');
        expect(SampleType.CNS).to.equals('CNS');
        expect(SampleType.Deco).to.equals('Deco');
        expect(SampleType.Depth).to.equals('Depth');
        expect(SampleType.Event).to.equals('Event');
        expect(SampleType.Gasmix).to.equals('Gasmix');
        expect(SampleType.Heartbeat).to.equals('Heartbeat');
        expect(SampleType.PPO2).to.equals('PPO2');
        expect(SampleType.Pressure).to.equals('Pressure');
        expect(SampleType.RBT).to.equals('RBT');
        expect(SampleType.Setpoint).to.equals('Setpoint');
        expect(SampleType.Temperature).to.equals('Temperature');
        expect(SampleType.Time).to.equals('Time');
        expect(SampleType.Vendor).to.equals('Vendor');
    });
});

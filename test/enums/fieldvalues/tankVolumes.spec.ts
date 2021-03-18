import { expect } from 'chai';
import { TankVolume } from 'libdivecomputer';

describe('WaterType', () => {
    it('Has all WaterType', () => {
        expect(TankVolume.Imperial).equals('Imperial');
        expect(TankVolume.Metric).equals('Metric');
        expect(TankVolume.None).equals('None');
    });
});

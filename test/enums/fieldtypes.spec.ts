import { expect } from 'chai';
import { FieldType } from 'libdivecomputerjs';

describe('FieldTypes', () => {
    it('Has all FieldTypes', () => {
        expect(FieldType.Atmospheric).to.equals('Atmospheric');
        expect(FieldType.AverageDepth).to.equals('AverageDepth');
        expect(FieldType.DiveMode).to.equals('DiveMode');
        expect(FieldType.DiveTime).to.equals('DiveTime');
        expect(FieldType.GasMix).to.equals('GasMix');
        expect(FieldType.GasMixCount).to.equals('GasMixCount');
        expect(FieldType.MaxDepth).to.equals('MaxDepth');
        expect(FieldType.Salinity).to.equals('Salinity');
        expect(FieldType.Tank).to.equals('Tank');
        expect(FieldType.TankCount).to.equals('TankCount');
        expect(FieldType.TemperatureMaximum).to.equals('TemperatureMaximum');
        expect(FieldType.TemperatureMinimum).to.equals('TemperatureMinimum');
        expect(FieldType.TemperatureSurface).to.equals('TemperatureSurface');
    });
});

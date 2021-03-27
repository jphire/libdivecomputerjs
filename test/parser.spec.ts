import { expect } from 'chai';
import { Context, Descriptor, FieldType, Parser } from 'libdivecomputerjs';
import { readFileSync } from 'fs';
import { getEonSteel } from '../examples/helpers/descriptors';
import { unbundleDiveData } from '../examples/helpers/parser';

describe('Parser', () => {
    let context: Context;
    let descriptor: Descriptor;

    let devtime: number;
    let systime: bigint;
    let diveData: Buffer;
    const referenceData = require('./data/SuuntoEONSteelReference.json');

    before(() => {
        let bundledData = readFileSync(
            __dirname + '/data/SuuntoEONSteelDump.bin'
        );
        const obj = unbundleDiveData(bundledData);
        devtime = obj.devtime;
        systime = obj.systime;
        diveData = obj.diveData;
        context = new Context();
        descriptor = getEonSteel()!;
    });

    it('can create a parser', () => {
        const parser = new Parser(context, descriptor, devtime, systime);
        expect(parser).not.null;
    });

    it('set divedata', () => {
        const parser = new Parser(context, descriptor, devtime, systime);
        parser.setData(diveData);
    });

    describe('Parser with data', () => {
        let parser: Parser;
        before(() => {
            parser = new Parser(context, descriptor, devtime, systime);
            parser.setData(diveData);
        });

        it('DateTime', () => {
            expect(parser.getDatetime()).to.equals(referenceData.datetime);
        });

        it('Field Atmospheric', () => {
            expect(parser.getField(FieldType.Atmospheric)).to.equals(
                referenceData.atmospheric
            );
        });

        it('Field DiveTime', () => {
            expect(parser.getField(FieldType.DiveTime)).to.equals(
                referenceData.divetime
            );
        });

        it('Field MaxDepth', () => {
            expect(parser.getField(FieldType.MaxDepth)).to.equals(
                referenceData.maxDepth
            );
        });

        it('Field Tank', () => {
            expect(parser.getField(FieldType.Tank)).to.deep.equals(
                referenceData.tank
            );
        });

        it('Field TankCount', () => {
            expect(parser.getField(FieldType.TankCount)).to.deep.equals(
                referenceData.tankCount
            );
        });

        it('Field Gasmix', () => {
            expect(parser.getField(FieldType.GasMix)).to.deep.equals(
                referenceData.gasmix
            );
        });

        it('Field GasmixCount', () => {
            expect(parser.getField(FieldType.GasMixCount)).to.equals(
                referenceData.gasmixCount
            );
        });

        it('Field AverageDepth', () => {
            expect(parser.getField(FieldType.AverageDepth)).to.be.undefined;
        });

        it('Field DiveMode', () => {
            expect(parser.getField(FieldType.DiveMode)).to.be.undefined;
        });

        it('Field Salinity', () => {
            expect(parser.getField(FieldType.Salinity)).to.be.undefined;
        });

        it('Field TemperatureMaximum', () => {
            expect(parser.getField(FieldType.TemperatureMaximum)).to.be
                .undefined;
        });

        it('Field TemperatureMinimum', () => {
            expect(parser.getField(FieldType.TemperatureMinimum)).to.be
                .undefined;
        });

        it('Field TemperatureSurface', () => {
            expect(parser.getField(FieldType.TemperatureSurface)).to.be
                .undefined;
        });

        it('Samples Count', () => {
            let iX = 0;
            parser.samplesForeach(() => iX++);
            expect(iX).to.equal(referenceData.samples.length);
        });

        it('Samples Content', () => {
            let iX = 0;
            parser.samplesForeach((sample) =>
                expect(sample).to.deep.equal(referenceData.samples[iX++])
            );
            expect(iX).to.equal(referenceData.samples.length);
        });
    });
});

import { expect } from 'chai';
import { DiveMode } from 'libdivecomputer';

describe('Divemode', () => {
    it('Has all divemodes', () => {
        expect(DiveMode.Freedive).equals('Freedive');
        expect(DiveMode.Gauge).equals('Gauge');
        expect(DiveMode.OpenCircuit).equals('OpenCircuit');
        expect(DiveMode.ClosedCircuitRebreather).equals(
            'ClosedCircuitRebreather'
        );
        expect(DiveMode.SemiclosedCircuitRebreather).equals(
            'SemiclosedCircuitRebreather'
        );
    });
});

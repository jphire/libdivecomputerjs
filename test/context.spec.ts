import chai, { expect } from 'chai';
import { Context, LogLevel, Transport } from 'libdivecomputerjs';
import assertType from 'chai-asserttype';
chai.use(assertType);

describe('Context', () => {
    it('it can create context', () => {
        new Context();
    });

    it('it can set loglevel', () => {
        const ctx = new Context();
        ctx.logLevel = LogLevel.All;
        expect(ctx.logLevel).to.equals(ctx.logLevel);
    });

    it('it can set and call log callback', () => {
        const ctx = new Context();
        let called = false;
        ctx.onLog((level: LogLevel, msg: string) => {
            expect(msg).to.be.string();
            expect(msg).to.equals(msg);
            expect(level).to.equals(LogLevel.Error);
            called = true;
        });

        ctx.log(LogLevel.Error, 'test');
    });

    it('Defaults to loglevel Error', () => {
        const ctx = new Context();
        expect(ctx.logLevel).to.equals(LogLevel.Error);
    });

    it('it can get transports', () => {
        const ctx = new Context();
        expect(ctx.transports).to.be.array();
        expect(ctx.transports.length).to.be.greaterThan(0);

        const invalidTransports = ctx.transports.filter((v) => !Transport[v]);
        expect(invalidTransports).lengthOf(
            0,
            'Expected only valid transports got the following invalid transports: ' +
                invalidTransports.join(', ')
        );
    });
});

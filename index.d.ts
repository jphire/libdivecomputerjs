declare module 'libdivecomputer' {
    export enum LogLevel {
        None,
        Error,
        Warning,
        Info,
        Debug,
        All,
    }

    export enum Transport {
        None,
        IRDA,
        USB,
        USBHID,
        Serial,
        Bluetooth,
        BLE,
    }

    export enum EventType {
        Clock = 'Clock',
        DevInfo = 'DevInfo',
        Progress = 'Progress',
        Waiting = 'Waiting',
        Vendor = 'Vendor',
    }

    export enum FieldType {
        Atmospheric,
        AverageDepth,
        DiveMode,
        DiveTime,
        GasMix,
        GasMixCount,
        MaxDepth,
        Salinity,
        Tank,
        TankCount,
        TemperatureMaximum,
        TemperatureMinimum,
        TemperatureSurface,
    }

    export enum DiveMode {
        Freedive,
        Gauge,
        OpenCircuit,
        ClosedCircuitRebreather,
        SemiclosedCircuitRebreather,
    }

    export enum TankVolume {
        None,
        Imperial,
        Metric,
    }

    export enum WaterType {
        Fresh,
        Salt,
    }

    export enum SampleType {
        Bearing,
        CNS,
        Deco,
        Depth,
        Event,
        Gasmix,
        Heartbeat,
        PPO2,
        Pressure,
        RBT,
        Setpoint,
        Temperature,
        Time,
        Vendor,
    }

    export enum SampleEventType {
        None,
        Deco,
        RBT,
        Ascent,
        Ceiling,
        Workload,
        Transmitter,
        Violation,
        Bookmark,
        Surface,
        SafetyStop,
        Gaschange,
        SafetyStopVoluntary,
        SafetyStopMandatory,
        Deepstop,
        CeilingSafetyStop,
        Floor,
        Divetime,
        Maxdepth,
        OLF,
        PO2,
        Airtime,
        RGBM,
        Heading,
        TissueLevelWarning,
        Gaschange2,
    }

    export class Descriptor {
        static iterate(): Iterable<Descriptor>;
        readonly product: string;
        readonly vendor: string;
        readonly model: number;
        readonly transports: Transports[];
    }

    export class USBHIDTransport {
        static iterate(
            context: Context,
            descriptor: Descriptor
        ): Iterable<USBHIDTransport>;
        readonly pid: number;
        readonly vid: number;
        open(context: Context): IOStream;
        toString(): string;
    }

    export class SerialTransport {
        static iterate(
            context: Context,
            descriptor: Descriptor
        ): Iterable<SerialTransport>;
        readonly name: string;
        open(context: Context): IOStream;
        toString(): string;
    }

    export class IRDATransport {
        static iterate(
            context: Context,
            descriptor: Descriptor
        ): Iterable<IRDATransport>;
        readonly name: string;
        readonly address: string;
        open(context: Context): IOStream;
        toString(): string;
    }

    export class BluetoothTranport {
        static iterate(
            context: Context,
            descriptor: Descriptor
        ): Iterable<BluetoothTranport>;
        readonly name: string;
        open(context: Context): IOStream;
        toString(): string;
    }

    class IOStream {}

    type EventData<T extends EventType, D extends object> = {
        type: T;
        data: D;
    };
    type EventsData =
        | EventData<EventType.Clock, { devtime: number; systime: bigint }>
        | EventData<
              EventType.DevInfo,
              { firmware: number; model: number; serial: number }
          >
        | EventData<EventType.Progress, { current: number; maximum: number }>
        | EventData<EventType.Waiting, void>;

    type EventCallback = (args: EventsData) => void;

    export class Device {
        constructor(
            context: Context,
            descriptor: Descriptor,
            iostream: IOStream
        );
        setFingerprint(data: Buffer): void;
        setEvents(events: EventType[], callback: EventCallback): void;
        setCancel(callback: () => boolean): void;
        foreach(
            callback: (diveData: Buffer, fingerprint: Buffer) => void | boolean
        ): void;
    }

    export class Context {
        logLevel: LogLevel;
        readonly transports: Transports[];
        log(logLevel: LogLevel, message: string);
        onLog(cb: (logLevel: LogLevel, message: string) => void);
    }

    type SampleInstance<T extends SampleType, K> = {
        type: T;
        value: K;
    };
    type Sample =
        | SampleInstance<
              | SampleType.Bearing
              | SampleType.CNS
              | SampleType.Depth
              | SampleType.Gasmix
              | SampleType.Heartbeat
              | SampleType.PPO2
              | SampleType.RBT
              | SampleType.Setpoint
              | SampleType.Temperature
              | SampleType.Time,
              number
          >
        | SampleInstance<SampleType.Vendor, { type: number; data: ArrayBuffer }>
        | SampleInstance<SampleType.Pressure, { tank: number; value: number }>
        | SampleInstance<
              SampleType.Deco,
              { depth: number; time: number; type: number }
          >;

    type NumbericFields =
        | FieldType.Atmospheric
        | FieldType.AverageDepth
        | FieldType.MaxDepth
        | FieldType.TemperatureSurface
        | FieldType.TemperatureMinimum
        | FieldType.TemperatureMaximum
        | FieldType.DiveTime
        | FieldType.TankCount
        | FieldType.GasMixCount;

    type TankValue = {
        type: TankVolume;
        beginPressure: number;
        endPressure: number;
        workingPressure: number;
        volume: number;
        gasmix: number;
    };
    export class Parser {
        static fromDevice(device: Device): Parser;
        static fromData(
            context: Context,
            descriptor: Descriptor,
            devtime: number,
            systime: bigint
        ): Parser;
        setData(data: Buffer): void;
        getField(field: NumbericFields): ?number;
        getField(field: FieldType.DiveMode): ?DiveMode;
        getField(
            field: FieldType.Salinity
        ): ?{ density: number; type: WaterType };
        getField(field: FieldType.Tank): ?TankValue;
        getField(
            field: FieldType.GasMix
        ): ?{ helium: number; oxygen: number; nitrogen: number };
        getDatetime(): string;
        samplesForeach(callback: (sample: Sample) => void): void;
    }

    export function version(): string;
}

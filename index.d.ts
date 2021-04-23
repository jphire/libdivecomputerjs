declare module 'libdivecomputerjs' {
    export enum LogLevel {
        None = 'None',
        Error = 'Error',
        Warning = 'Warning',
        Info = 'Info',
        Debug = 'Debug',
        All = 'All',
    }

    export enum Transport {
        None = 'None',
        IRDA = 'IRDA',
        USB = 'USB',
        USBHID = 'USBHID',
        Serial = 'Serial',
        Bluetooth = 'Bluetooth',
        BLE = 'BLE',
    }

    export enum EventType {
        Clock = 'Clock',
        DevInfo = 'DevInfo',
        Progress = 'Progress',
        Waiting = 'Waiting',
        Vendor = 'Vendor',
    }

    export enum FieldType {
        Atmospheric = 'Atmospheric',
        AverageDepth = 'AverageDepth',
        DiveMode = 'DiveMode',
        DiveTime = 'DiveTime',
        GasMix = 'GasMix',
        GasMixCount = 'GasMixCount',
        MaxDepth = 'MaxDepth',
        Salinity = 'Salinity',
        Tank = 'Tank',
        TankCount = 'TankCount',
        TemperatureMaximum = 'TemperatureMaximum',
        TemperatureMinimum = 'TemperatureMinimum',
        TemperatureSurface = 'TemperatureSurface',
    }

    export enum DiveMode {
        Freedive = 'Freedive',
        Gauge = 'Gauge',
        OpenCircuit = 'OpenCircuit',
        ClosedCircuitRebreather = 'ClosedCircuitRebreather',
        SemiclosedCircuitRebreather = 'SemiclosedCircuitRebreather',
    }

    export enum TankVolume {
        None = 'None',
        Imperial = 'Imperial',
        Metric = 'Metric',
    }

    export enum WaterType {
        Fresh = 'Fresh',
        Salt = 'Salt',
    }

    export class AsyncDeviceReader {
        setContext(context: Context): void;
        setDescriptor(descriptor: Descriptor): void;
        setTransport(
            transport:
                | USBHIDTransport
                | SerialTransport
                | IRDATransport
                | BluetoothTranport
        ): void;
        onEvents(events: EventType[], callback: EventCallback): void;
        onDive(callback: (diveData: Buffer, fingerprint: Buffer) => void): void;
        read(cb: (err?: Error) => void): void;
    }

    export function test(
        dive: (dive: Buffer, fingerprint: Buffer) => void,
        cb: () => void
    );

    export enum SampleType {
        Bearing = 'Bearing',
        CNS = 'CNS',
        Deco = 'Deco',
        Depth = 'Depth',
        Event = 'Event',
        Gasmix = 'Gasmix',
        Heartbeat = 'Heartbeat',
        PPO2 = 'PPO2',
        Pressure = 'Pressure',
        RBT = 'RBT',
        Setpoint = 'Setpoint',
        Temperature = 'Temperature',
        Time = 'Time',
        Vendor = 'Vendor',
    }

    export enum SampleEventType {
        None = 'None',
        Deco = 'Deco',
        RBT = 'RBT',
        Ascent = 'Ascent',
        Ceiling = 'Ceiling',
        Workload = 'Workload',
        Transmitter = 'Transmitter',
        Violation = 'Violation',
        Bookmark = 'Bookmark',
        Surface = 'Surface',
        SafetyStop = 'SafetyStop',
        Gaschange = 'Gaschange',
        SafetyStopVoluntary = 'SafetyStopVoluntary',
        SafetyStopMandatory = 'SafetyStopMandatory',
        Deepstop = 'Deepstop',
        CeilingSafetyStop = 'CeilingSafetyStop',
        Floor = 'Floor',
        Divetime = 'Divetime',
        Maxdepth = 'Maxdepth',
        OLF = 'OLF',
        PO2 = 'PO2',
        Airtime = 'Airtime',
        RGBM = 'RGBM',
        Heading = 'Heading',
        TissueLevelWarning = 'TissueLevelWarning',
        Gaschange2 = 'Gaschange2',
    }

    export class Descriptor {
        static iterate(): Iterable<Descriptor>;
        readonly product: string;
        readonly vendor: string;
        readonly model: number;
        readonly transports: Transport[];
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
        | { type: EventType.Waiting };

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
            itemCallback: (diveData: Buffer, fingerprint: Buffer) => void
        ): void;
    }

    export class Context {
        logLevel: LogLevel;
        readonly transports: Transport[];
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
        constructor(device: Device);
        constructor(
            context: Context,
            descriptor: Descriptor,
            devtime: number,
            systime: bigint
        );
        setData(data: Buffer): void;
        getField(field: NumbericFields): undefined | number;
        getField(field: FieldType.DiveMode): undefined | DiveMode;
        getField(
            field: FieldType.Salinity
        ): undefined | { density: number; type: WaterType };
        getField(field: FieldType.Tank): undefined | TankValue;
        getField(
            field: FieldType.GasMix
        ): undefined | { helium: number; oxygen: number; nitrogen: number };
        getDatetime(): string;
        samplesForeach(callback: (sample: Sample) => void): void;
    }

    export function version(): string;
}

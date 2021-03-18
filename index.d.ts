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
        Clock,
        DevInfo,
        Progress,
        Waiting,
        Vendor,
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

    export class Descriptor {
        static iterate(): Iterable<Descriptor>;
        readonly product: string;
        readonly vendor: string;
        readonly model: number;
        readonly transports: Transports[];
    }

    export class USBHIDDevice {
        static iterate(): Iterable<USBHIDDevice>;
        readonly pid: number;
        readonly vid: number;
        toString(): string;
        open(): IOStream;
    }
    export class SerialDevice {
        static iterate(): Iterable<SerialDevice>;
        readonly name: string;
        toString(): string;
        open(): IOStream;
    }
    export class IRDADevice {
        static iterate(): Iterable<SerialDevice>;
        readonly name: string;
        readonly address: string;
        toString(): string;
        open(): IOStream;
    }
    export class BluetoothDevice {
        static iterate(): Iterable<SerialDevice>;
        readonly name: string;
        toString(): string;
        open(): IOStream;
    }
    class IOStream {}

    type EventData<T extends EventType, D extends object> = {
        event: T;
        data: D;
    };
    type EventsData =
        | EventData<EventType.Clock, { devtime: number; systime: number }>
        | EventData<
              EventType.DevInfo,
              { firmware: number; model: number; serial: number }
          >
        | EventData<EventType.Progress, { current: number; maximum: number }>
        | EventData<EventType.Waiting, void>;

    type EventCallback = (args: EventsData) => void;

    export class Device {
        setFingerprint(data: ArrayBuffer): void;
        setEvents(events: EventType[], callback: EventCallback): void;
    }

    export class Context {
        logLevel: LogLevel;
        readonly transports: Transports[];
        log(logLevel: LogLevel, message: string);
        onLog(cb: (logLevel: LogLevel, message: string) => void);
    }

    export function version(): string;
}

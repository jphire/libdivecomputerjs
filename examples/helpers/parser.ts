export function unbundleDiveData(
    bundledData: Buffer
): { devtime: number; systime: bigint; diveData: Buffer } {
    const devtime = bundledData.readUInt32BE(0);
    const systime = bundledData.readBigInt64BE(4);

    return { devtime, systime, diveData: bundledData.slice(12) };
}

export function bundleDiveData(
    diveData: Buffer,
    devtime: number,
    systime: bigint
): Buffer {
    const biggerBuffer = Buffer.allocUnsafe(diveData.length + 4 + 8);

    biggerBuffer.writeUInt32BE(devtime, 0);
    biggerBuffer.writeBigInt64BE(systime, 4);
    diveData.copy(biggerBuffer, 12);

    return biggerBuffer;
}

#pragma once

template <class TWrap, class TValue>
class WrappedData
{
public:
    WrappedData(TWrap *wrap, TValue *data);
    ~WrappedData();

    TValue *Data();
    TWrap *Wrap();

private:
    TWrap *wrap;
    TValue *data;
};

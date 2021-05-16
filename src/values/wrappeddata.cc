#include "./wrappeddata.h"
#include <libdivecomputer/device.h>
#include "../contracts/errorable.h"
#include "./eventdata.h"
#include "./divedata.h"
#include "./logdata.h"

template <class TWrap, class TValue>
WrappedData<TWrap, TValue>::WrappedData(TWrap *wrap, TValue *data)
    : wrap(wrap), data(data)
{
}

template <class TWrap, class TValue>
WrappedData<TWrap, TValue>::~WrappedData()
{
    delete data;
}

template <class TWrap, class TValue>
TValue *WrappedData<TWrap, TValue>::Data()
{
    return data;
}

template <class TWrap, class TValue>
TWrap *WrappedData<TWrap, TValue>::Wrap()
{
    return wrap;
}

template class WrappedData<Errorable, Eventdata>;
template class WrappedData<Errorable, Divedata>;
template class WrappedData<Errorable, Logdata>;
template class WrappedData<Errorable, dc_device_t>;
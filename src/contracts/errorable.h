#include <napi.h>

class Errorable
{
public:
    virtual void setError(const Napi::Error &error) = 0;
};
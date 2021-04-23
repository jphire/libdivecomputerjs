#pragma once
#include <cstdlib>

class Divedata
{
public:
    unsigned char *divedata = NULL;
    size_t divedatasize = 0;
    unsigned char *fingerprint = NULL;
    size_t fingerprintsize = 0;

    Divedata(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize);
    ~Divedata();
};

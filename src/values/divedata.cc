#include "divedata.h"
#include <stdio.h>
#include <cstring>

Divedata::Divedata(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize)
{
    this->divedatasize = size;
    this->divedata = (unsigned char *)calloc(size, sizeof(const unsigned char));
    memcpy((void *)this->divedata, data, size);

    this->fingerprintsize = fsize;
    this->fingerprint = (unsigned char *)calloc(size, sizeof(const unsigned char));
    memcpy((void *)this->fingerprint, fingerprint, fsize);
}

Divedata::~Divedata()
{
    if (divedata != NULL)
    {
        free(divedata);
        divedata = NULL;
    }

    if (fingerprint != NULL)
    {
        free(fingerprint);
        fingerprint = NULL;
    }
}
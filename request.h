#ifndef REQUEST_H
#define REQUEST_H

#include "common.h"

struct TRequest
{
    //! Petrol type.
    int Type;

    //! Amount of petrol to buy.
    double Amount;

    //! Time of car arrival.
    TTime StartTime;

    //! Time of service start.
    TTime ProcessTime = -1;

    //! Time of service end.
    TTime EndTime = -1;

    TTime RequestProcessingTime() const
    {
        return Amount * 0.7;
    }
};

#endif // REQUEST_H

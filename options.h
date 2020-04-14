#ifndef TOPTIONS_H
#define TOPTIONS_H

#include "common.h"
#include <vector>

struct TPetrol
{
    //! Price per liter (without markup).
    double Price = 1;

    //! Markup of petrol cost (in percent)
    double Markup = 0;

    double GetPrice() const
    {
        return Price * (1 + Markup / 100);
    }
};

struct TOptions
{
    //! Number of queues processing requests.
    int QueueCount = 1;

    //! Maximum size of queue.
    int MaxQueueSize = 10;

    //! Period of one step of simulation (in minutes).
    TTime SimulationPeriod = 10;

    //! Petrol types descriptions.
    std::vector<TPetrol> Petrols;
};

#endif // TOPTIONS_H

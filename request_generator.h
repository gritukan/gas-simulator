#ifndef REQUEST_GENERATOR_H
#define REQUEST_GENERATOR_H

#include "common.h"
#include "options.h"
#include "request.h"

#include <ctime>
#include <exception>

class TRequestGenerator
{
public:
    TRequestGenerator(TOptions options)
        : Options_(std::move(options))
    { }

    std::vector<TRequest> GenerateRequests() const
    {
        srand(time(nullptr));
        std::vector<TRequest> requests;
        TTime time = 0;
        while (time < Week) {
            TTime nextRequestTime = time + rand() % CarArrivalFrequency(time);
            time = nextRequestTime;
            TTime nextRequestPetrolType = rand() % Options_.Petrols.size();
            double nextRequestPetrolAmount = 10 + static_cast<double>(rand() % 300) / 10;
            // Petrol is too expensive, ignore this request.
            if (rand() % 100 <= Options_.Petrols[nextRequestPetrolType].Markup) {
                continue;
            }
            TRequest request;
            request.Type = nextRequestPetrolType;
            request.Amount = nextRequestPetrolAmount;
            request.StartTime = nextRequestTime;

            requests.push_back(request);
        }

        return requests;
    }

private:
    //! Returns average time between two consecutive requests
    //! for some time of the day.
    static int CarArrivalFrequency(TTime time)
    {
        time %= Day;
        if (time >= 0 * Hour && time < 6 * Hour) {
            return 20;
        } else if (time >= 6 * Hour && time < 9 * Hour) {
            return 10;
        } else if (time >= 9 * Hour && time < 18 * Hour) {
            return 5;
        } else if (time >= 18 * Hour && time < 24 * Hour) {
            return 15;
        }
        throw std::exception();
    }
    TOptions Options_;
};

#endif // REQUEST_GENERATOR_H

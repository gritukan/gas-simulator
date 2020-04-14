#ifndef STATISTICS_H
#define STATISTICS_H

#include <common.h>
#include <options.h>
#include <request.h>

#include <string>
#include <sstream>

class TPetrolStatistics
{
public:
    TPetrolStatistics() = default;

    TPetrolStatistics(TPetrol petrol)
        : Petrol_(std::move(petrol))
    { }

    void OnPetrolSold(TTime time, double amount)
    {
        Events_.emplace_back(time, amount);
    }

    double PetrolSold(TTime startTime, TTime endTime) const
    {
        double sold = 0;
        for (const auto& event : Events_) {
            if (event.Time >= startTime && event.Time <= endTime) {
                sold += event.Amount;
            }
        }
        return sold;
    }

private:
    struct TPetrolSoldEvent
    {
        TPetrolSoldEvent(TTime time, double amount)
            : Time(time)
            , Amount(amount)
        { }

        TTime Time;
        double Amount;
    };
    std::vector<TPetrolSoldEvent> Events_;

    TPetrol Petrol_;
};

class TStatistics
{
public:
    TStatistics() = default;

    TStatistics(TOptions options)
        : Options_(std::move(options))
    {
        PetrolStatistics_.resize(Options_.Petrols.size());
        for (size_t index = 0; index < PetrolStatistics_.size(); ++index) {
            PetrolStatistics_[index] = TPetrolStatistics{Options_.Petrols[index]};
        }
    }

    void OnPetrolSold(const TRequest& request)
    {
        ++CarsProcessed_;
        TotalProfit_ += Options_.Petrols[request.Type].GetPrice() * request.Amount;
        PetrolStatistics_[request.Type].OnPetrolSold(request.ProcessTime, request.Amount);
    }

    void OnCarRejected()
    {
        ++CarsRejected_;
    }

    int GetCarsProcessed() const
    {
        return CarsProcessed_;
    }

    std::string Print(TTime time) const
    {
        std::stringstream result;
        result << "Time: " << PrintTime(time) << std::endl;
        result << "Cars processed: " << CarsProcessed_ << std::endl;
        result << "Cars rejected: " << CarsRejected_ << std::endl;
        result << "Total profit: " << TotalProfit_ << std::endl;
        for (size_t petrolIndex = 0; petrolIndex < PetrolStatistics_.size(); ++petrolIndex) {
            result << "Petrol " << petrolIndex + 1 << ":" << std::endl;
            result << "Total sold: " << PetrolStatistics_[petrolIndex].PetrolSold(0, Week) << std::endl;
            result << "Sold during last day: " << PetrolStatistics_[petrolIndex].PetrolSold(std::max<int>(0, time - Day), time) << std::endl;
        }

        return result.str();
    }

private:
    TOptions Options_;
    std::vector<TPetrolStatistics> PetrolStatistics_;

    int CarsProcessed_ = 0;
    int CarsRejected_ = 0;
    double TotalProfit_ = 0;
};

#endif // STATISTICS_H

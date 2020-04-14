#ifndef STATE_H
#define STATE_H

#include "request.h"
#include "statistics.h"

#include <iostream>
#include <queue>

class TState
{
public:
    TState() = default;

    TState(std::vector<TRequest> requests, TOptions options)
        : Options_(std::move(options))
        , Statistics_(Options_)
        , AllRequests_(std::move(requests))
        , Queues_(Options_.QueueCount)
        , QueueTimes_(Options_.QueueCount, 0)
    { }

    //! Processes all the events until the moment `time'.
    void Simulate(TTime time)
    {
        while (RequestPtr_ < AllRequests_.size() && AllRequests_[RequestPtr_].StartTime <= time) {
            const auto& request = AllRequests_[RequestPtr_];
            for (int queueIndex = 0; queueIndex < Queues_.size(); ++queueIndex) {
                ProcessEvents(queueIndex, request.StartTime);
            }
            AddRequest(request);
            ++RequestPtr_;
        }

        for (int queueIndex = 0; queueIndex < Queues_.size(); ++queueIndex) {
            ProcessEvents(queueIndex, time);
        }
    }

    std::vector<int> GetQueueSizes() const
    {
        std::vector<int> queueSizes;
        queueSizes.reserve(Queues_.size());
        for (const auto& queue : Queues_) {
            queueSizes.push_back(queue.size());
        }
        return queueSizes;
    }

    const TStatistics& GetStatistics() const
    {
        return Statistics_;
    }

private:
    void AddRequest(const TRequest& request)
    {
        int bestQueueIndex = 0;
        for (int queueIndex = 1; queueIndex < Queues_.size(); ++queueIndex) {
            if (Queues_[queueIndex].size() < Queues_[bestQueueIndex].size()) {
                bestQueueIndex = queueIndex;
            }
        }
        if (Queues_[bestQueueIndex].size() >= Options_.MaxQueueSize) {
            Statistics_.OnCarRejected();
        } else {
            Queues_[bestQueueIndex].push(request);
        }
    }

    void ProcessEvents(int queueIndex, int time)
    {
        auto& queue = Queues_[queueIndex];
        int& queueTime = QueueTimes_[queueIndex];

        while (!queue.empty()) {
            auto& request = queue.front();
            if (request.ProcessTime == -1) {
                request.ProcessTime = std::max(request.StartTime, queueTime);
                request.EndTime = request.ProcessTime + request.RequestProcessingTime();
            }
            queueTime = std::max(queueTime, request.ProcessTime);
            if (request.EndTime > time) {
                break;
            }
            Statistics_.OnPetrolSold(request);
            queueTime = std::max(queueTime, request.EndTime);
            queue.pop();
        }
    }

    TOptions Options_;
    TStatistics Statistics_;
    std::vector<TRequest> AllRequests_;
    size_t RequestPtr_ = 0;
    std::vector<std::queue<TRequest>> Queues_;
    std::vector<int> QueueTimes_;
};

#endif // STATE_H

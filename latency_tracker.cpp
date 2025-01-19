#include "latency_tracker.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
using namespace std;

vector<LatencyTracker::LatencyRecord> LatencyTracker::latencyHistory;
mutex LatencyTracker::historyMutex;
bool LatencyTracker::detailedLogging = false;

chrono::high_resolution_clock::time_point LatencyTracker::startMeasurement()
{
    return chrono::high_resolution_clock::now();
}

void LatencyTracker::endMeasurement(const chrono::high_resolution_clock::time_point &startTime,
                                    const string &operationName)
{
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;

    recordLatency(operationName, duration.count());

    if (detailedLogging)
    {
        cout << operationName << " completed in " << duration.count() * 1000.0 << " ms" << endl;
    }
}

double LatencyTracker::getAverageLatency(const string &operationType)
{
    lock_guard<mutex> lock(historyMutex);

    auto matchingRecords = count_if(latencyHistory.begin(), latencyHistory.end(),
                                    [&](const LatencyRecord &record)
                                    {
                                        return record.operation == operationType;
                                    });

    if (matchingRecords == 0)
        return 0.0;

    double totalLatency = accumulate(latencyHistory.begin(), latencyHistory.end(), 0.0,
                                     [&](double sum, const LatencyRecord &record)
                                     {
                                         return sum + (record.operation == operationType ? record.duration : 0);
                                     });

    return totalLatency / matchingRecords;
}

void LatencyTracker::resetStatistics()
{
    lock_guard<mutex> lock(historyMutex);
    latencyHistory.clear();
}

void LatencyTracker::enableDetailedLogging(bool enable)
{
    detailedLogging = enable;
}

void LatencyTracker::recordLatency(const string &operation, double duration)
{
    lock_guard<mutex> lock(historyMutex);

    LatencyRecord record{
        operation,
        duration,
        chrono::system_clock::now()};

    latencyHistory.push_back(record);

    if (latencyHistory.size() > 1000)
    {
        latencyHistory.erase(latencyHistory.begin());
    }
}

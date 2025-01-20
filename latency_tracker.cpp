#include "latency_tracker.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
using namespace std;

vector<LatencyTracker::LatencyRecord> LatencyTracker::latencyHistory;
mutex LatencyTracker::historyMutex;
bool LatencyTracker::detailedLogging = true;
map<string, LatencyTracker::OperationStats> LatencyTracker::operationStats;

chrono::high_resolution_clock::time_point LatencyTracker::startMeasurement(const string &operationType)
{
    if (!operationType.empty() && detailedLogging)
    {
        cout << "\n=== Starting " << operationType << " ===\n";
    }
    return chrono::high_resolution_clock::now();
}

void LatencyTracker::endMeasurement(const chrono::high_resolution_clock::time_point &startTime,
                                    const string &operationName)
{
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;
    double milliseconds = duration.count() * 1000.0;

    recordLatency(operationName, milliseconds);
    updateStats(operationName, milliseconds);

    cout << "\n=== Latency Metrics ===\n";
    cout << operationName << ":\n";
    cout << "  Current: " << fixed << setprecision(2) << milliseconds << " ms\n";

    lock_guard<mutex> lock(historyMutex);
    auto it = operationStats.find(operationName);
    if (it != operationStats.end())
    {
        const auto &stats = it->second;
        cout << "  Average: " << stats.total / stats.count << " ms\n";
        cout << "  Min: " << stats.min << " ms\n";
        cout << "  Max: " << stats.max << " ms\n";
        cout << "  Sample Count: " << stats.count << "\n";
    }
}

void LatencyTracker::displayLatencyStats()
{
    lock_guard<mutex> lock(historyMutex);
    cout << "\n=== Overall Latency Statistics ===\n";
    for (const auto &[operation, stats] : operationStats)
    {
        cout << "\n"
             << operation << ":\n";
        cout << "  Average: " << fixed << setprecision(2) << stats.total / stats.count << " ms\n";
        cout << "  Min: " << stats.min << " ms\n";
        cout << "  Max: " << stats.max << " ms\n";
        cout << "  Sample Count: " << stats.count << "\n";
    }
}

double LatencyTracker::getAverageLatency(const string &operationType)
{
    lock_guard<mutex> lock(historyMutex);
    auto it = operationStats.find(operationType);
    if (it != operationStats.end())
    {
        const auto &stats = it->second;
        return stats.count > 0 ? stats.total / stats.count : 0.0;
    }
    return 0.0;
}

void LatencyTracker::resetStatistics()
{
    lock_guard<mutex> lock(historyMutex);
    latencyHistory.clear();
    operationStats.clear();
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

void LatencyTracker::updateStats(const string &operation, double duration)
{
    lock_guard<mutex> lock(historyMutex);

    auto &stats = operationStats[operation];
    stats.min = min(stats.min, duration);
    stats.max = max(stats.max, duration);
    stats.total += duration;
    stats.count++;
}

map<string, double> LatencyTracker::getLatencyAverages()
{
    lock_guard<mutex> lock(historyMutex);
    map<string, double> averages;

    for (const auto &[operation, stats] : operationStats)
    {
        averages[operation] = stats.count > 0 ? stats.total / stats.count : 0.0;
    }

    return averages;
}

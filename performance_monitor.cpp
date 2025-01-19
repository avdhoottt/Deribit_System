// performance_monitor.cpp
#include "performance_monitor.hpp"
#include <iostream>
#include <numeric>
using namespace std;

mutex PerformanceMonitor::metricsMutex;
map<string, vector<double>> PerformanceMonitor::operationMetrics;
bool PerformanceMonitor::profilingEnabled = false;

chrono::high_resolution_clock::time_point PerformanceMonitor::startTimer()
{
    return chrono::high_resolution_clock::now();
}

void PerformanceMonitor::stopTimer(const chrono::high_resolution_clock::time_point &startTime,
                                   const string &operationType)
{
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;
    double milliseconds = duration.count() * 1000.0;

    updateMetrics(operationType, milliseconds);

    if (profilingEnabled)
    {
        cout << operationType << " took " << milliseconds << " ms" << endl;
    }
}

void PerformanceMonitor::enableProfiling(bool enable)
{
    profilingEnabled = enable;
}

map<string, double> PerformanceMonitor::getAverageTimings()
{
    lock_guard<mutex> lock(metricsMutex);
    map<string, double> averages;

    for (const auto &[operation, timings] : operationMetrics)
    {
        if (!timings.empty())
        {
            double sum = accumulate(timings.begin(), timings.end(), 0.0);
            averages[operation] = sum / timings.size();
        }
    }

    return averages;
}

void PerformanceMonitor::clearMetrics()
{
    lock_guard<mutex> lock(metricsMutex);
    operationMetrics.clear();
}

void PerformanceMonitor::updateMetrics(const string &operation, double duration)
{
    lock_guard<mutex> lock(metricsMutex);
    operationMetrics[operation].push_back(duration);

    if (operationMetrics[operation].size() > 1000)
    {
        operationMetrics[operation].erase(operationMetrics[operation].begin());
    }
}

#ifndef PERFORMANCE_MONITOR_HPP
#define PERFORMANCE_MONITOR_HPP

#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <mutex>

using namespace std;

class PerformanceMonitor
{
public:
    static chrono::high_resolution_clock::time_point startTimer();
    static void stopTimer(const chrono::high_resolution_clock::time_point &startTime,
                          const string &operationType);

    static void enableProfiling(bool enable);
    static map<string, double> getAverageTimings();
    static void clearMetrics();

private:
    static mutex metricsMutex;
    static map<string, vector<double>> operationMetrics;
    static bool profilingEnabled;

    static void updateMetrics(const string &operation, double duration);
};

#endif

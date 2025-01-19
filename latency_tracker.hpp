#ifndef LATENCY_TRACKER_HPP
#define LATENCY_TRACKER_HPP

#include <chrono>
#include <string>
#include <vector>
#include <mutex>

using namespace std;

class LatencyTracker
{
public:
    static chrono::high_resolution_clock::time_point startMeasurement();
    static void endMeasurement(const chrono::high_resolution_clock::time_point &startTime,
                               const string &operationName);

    static double getAverageLatency(const string &operationType);
    static void resetStatistics();
    static void enableDetailedLogging(bool enable);

private:
    struct LatencyRecord
    {
        string operation;
        double duration;
        chrono::system_clock::time_point timestamp;
    };

    static vector<LatencyRecord> latencyHistory;
    static mutex historyMutex;
    static bool detailedLogging;

    static void recordLatency(const string &operation, double duration);
};

#endif

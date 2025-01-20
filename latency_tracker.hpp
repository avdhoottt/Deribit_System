#ifndef LATENCY_TRACKER_HPP
#define LATENCY_TRACKER_HPP

#include <chrono>
#include <string>
#include <vector>
#include <mutex>
#include <map>
#include <iomanip>

using namespace std;

class LatencyTracker
{
public:
    static chrono::high_resolution_clock::time_point startMeasurement(const string &operationType = "");
    static void endMeasurement(const chrono::high_resolution_clock::time_point &startTime,
                               const string &operationName);

    static void displayLatencyStats();
    static double getAverageLatency(const string &operationType);
    static void resetStatistics();
    static void enableDetailedLogging(bool enable);
    static map<string, double> getLatencyAverages();

private:
    struct LatencyRecord
    {
        string operation;
        double duration;
        chrono::system_clock::time_point timestamp;
    };

    struct OperationStats
    {
        double min = numeric_limits<double>::max();
        double max = 0;
        double total = 0;
        int count = 0;
    };

    static vector<LatencyRecord> latencyHistory;
    static mutex historyMutex;
    static bool detailedLogging;
    static map<string, OperationStats> operationStats;

    static void recordLatency(const string &operation, double duration);
    static void updateStats(const string &operation, double duration);
};

#endif

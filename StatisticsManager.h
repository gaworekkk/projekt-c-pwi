#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <string>

class StatisticsManager {
public:
    static void saveStatistics(int coinCount); 
    static void loadStatistics(int& coinCount); 
};

#endif
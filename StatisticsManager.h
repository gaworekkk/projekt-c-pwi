#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class StatisticsManager {
public:
    static void saveStatistics(int coinCount, int bestDistance, int totalDistance, int jumpCount, int deathCount, int gamesPlayed, int skinState[6]);
    static void loadStatistics(int& coinCount, int& bestDistance, int& totalDistance, int& jumpCount, int& deathCount, int& gamesPlayed, int skinState[6]);
};

#endif

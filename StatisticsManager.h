#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class StatisticsManager {
public:
    static void saveStatistics(int coinCount, float bestDistance, float totalDistance, int jumpCount, int deathCount, int gamesPlayed);
    static void loadStatistics(int& coinCount, float& bestDistance, float& totalDistance, int& jumpCount, int& deathCount, int& gamesPlayed);
};

#endif

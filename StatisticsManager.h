#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <string>
#include "Difficulty.h"
#include "json.hpp"

using json = nlohmann::json;

class StatisticsManager {
public:
    static void saveStatistics(int coinCount, float bestDistance, float totalDistance, int jumpCount, int deathCount, int gamesPlayed, int skinState[6], float musicVolume, float soundVolume, Difficulty difficulty);
    static void loadStatistics(int& coinCount, float& bestDistance, float& totalDistance, int& jumpCount, int& deathCount, int& gamesPlayed, int skinState[6], float& musicVolume, float& soundVolume, Difficulty& difficulty);
};

#endif

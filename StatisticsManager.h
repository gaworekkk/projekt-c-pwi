#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <string>
#include "Difficulty.h"
#include "json.hpp"

using json = nlohmann::json;

class StatisticsManager {
public:
    static void saveStatistics(int coinCount, int bestDistance, int totalDistance, int jumpCount, int deathCount, int gamesPlayed, int cactusCount, int birdCount, int skinState[6], float musicVolume, float soundVolume, Difficulty difficulty);
    static void loadStatistics(int& coinCount, int& bestDistance, int& totalDistance, int& jumpCount, int& deathCount, int& gamesPlayed, int& cactusCount, int& birdCount, int skinState[6], float& musicVolume, float& soundVolume, Difficulty& difficulty);
};

#endif

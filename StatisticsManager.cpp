#include "StatisticsManager.h"
#include <fstream>

void StatisticsManager::saveStatistics(int coinCount, float bestDistance, int jumpCount) {
    json statsJson = { {"coin_count", coinCount}, {"best_distance", bestDistance}, {"jump_count", jumpCount} };
    std::ofstream file("stats.json");
    if (file.is_open()) {
        file << statsJson.dump();
        file.close();
    }
}

void StatisticsManager::loadStatistics(int& coinCount, float& bestDistance, int& jumpCount) {
    std::ifstream file("stats.json");
    if (file.is_open()) {
        json statsJson;
        file >> statsJson;
        coinCount = statsJson.value("coin_count", 0);
        bestDistance = statsJson.value("best_distance", 0.0f);
        jumpCount = statsJson.value("jump_count", 0);
        file.close();
    } else {
        coinCount = 0; 
        bestDistance = 0.0f;
        jumpCount = 0;
    }
}

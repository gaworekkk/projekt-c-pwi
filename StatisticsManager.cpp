#include "StatisticsManager.h"
#include <fstream>

void StatisticsManager::saveStatistics(int coinCount) {
    json statsJson = { {"coin_count", coinCount} };
    std::ofstream file("stats.json");
    if (file.is_open()) {
        file << statsJson.dump();
        file.close();
    }
}

void StatisticsManager::loadStatistics(int& coinCount) {
    std::ifstream file("stats.json");
    if (file.is_open()) {
        json statsJson;
        file >> statsJson;
        coinCount = statsJson.value("coin_count", 0);
        file.close();
    } else {
        coinCount = 0; 
    }
}

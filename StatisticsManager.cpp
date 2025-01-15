#include "StatisticsManager.h"
#include <fstream>

void StatisticsManager::saveStatistics(int coinCount, float bestDistance, float totalDistance, int jumpCount, int deathCount, int gamesPlayed) {
    json statsJson = { 
        {"coin_count", coinCount}, 
        {"best_distance", bestDistance}, 
        {"total_distance", totalDistance}, 
        {"jump_count", jumpCount},
        {"death_count", deathCount},
        {"games_played", gamesPlayed}
    };
    std::ofstream file("stats.json");
    if (file.is_open()) {
        file << statsJson.dump();
        file.close();
    }
}

void StatisticsManager::loadStatistics(int& coinCount, float& bestDistance, float& totalDistance, int& jumpCount, int& deathCount, int& gamesPlayed) {
    std::ifstream file("stats.json");
    if (file.is_open()) {
        json statsJson;
        file >> statsJson;
        coinCount = statsJson.value("coin_count", 0);
        bestDistance = statsJson.value("best_distance", 0.0f);
        totalDistance = statsJson.value("total_distance", 0.0f);
        jumpCount = statsJson.value("jump_count", 0);
        deathCount = statsJson.value("death_count", 0);
        gamesPlayed = statsJson.value("games_played", 0);
        file.close();
    } else {
        coinCount = 0; 
        bestDistance = 0.0f;
        totalDistance = 0.0f;
        jumpCount = 0;
        deathCount = 0;
        gamesPlayed = 0;
    }
}

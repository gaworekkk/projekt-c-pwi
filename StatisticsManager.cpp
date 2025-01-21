#include "StatisticsManager.h"
#include <fstream>

void StatisticsManager::saveStatistics(int coinCount, int bestDistance, int totalDistance, int jumpCount, int deathCount, int gamesPlayed, int cactusCount, int birdCount, int skinState[6], float musicVolume, float soundVolume, Difficulty difficulty) {
    json statsJson = { 
        {"coin_count", coinCount}, 
        {"best_distance", bestDistance}, 
        {"total_distance", totalDistance}, 
        {"jump_count", jumpCount},
        {"death_count", deathCount},
        {"games_played", gamesPlayed},
        {"cactus_count", cactusCount},
        {"bird_count", birdCount},
        {"Dino", skinState[0]},
        {"Rascal", skinState[1]},
        {"Albino", skinState[2]},
        {"Smurf", skinState[3]},
        {"Shadow", skinState[4]},
        {"Dragon", skinState[5]},
        {"music_volume", musicVolume},
        {"sound_volume", soundVolume},
        {"difficulty", difficulty}
        
    };
    std::ofstream file("stats.json");
    if (file.is_open()) {
        file << statsJson.dump();
        file.close();
    }
}

void StatisticsManager::loadStatistics(int& coinCount, int& bestDistance, int& totalDistance, int& jumpCount, int& deathCount, int& gamesPlayed, int& cactusCount, int& birdCount, int skinState[6], float& musicVolume, float& soundVolume, Difficulty& difficulty) {
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
        cactusCount = statsJson.value("cactus_count", 0);
        birdCount = statsJson.value("bird_count", 0);
        skinState[0] = statsJson.value("Dino", 0);
        skinState[1] = statsJson.value("Rascal", 2);
        skinState[2] = statsJson.value("Albino", 2);
        skinState[3] = statsJson.value("Smurf", 2);
        skinState[4] = statsJson.value("Shadow", 2);
        skinState[5] = statsJson.value("Dragon", 2);
        musicVolume = statsJson.value("music_volume", 100);
        soundVolume = statsJson.value("sound_volume", 100);
        difficulty = statsJson.value("difficulty", Difficulty::Normal);
        file.close();
    } else {
        coinCount = 0; 
        bestDistance = 0.0f;
        totalDistance = 0.0f;
        jumpCount = 0;
        deathCount = 0;
        gamesPlayed = 0;
        cactusCount = 0;
        birdCount = 0;
        skinState[0] = 0;
        musicVolume = 100;
        soundVolume = 100;
        difficulty = Difficulty::Normal;
        for (int i = 1; i < 6; ++i) {
            skinState[i] = 2;
        }
    }
}

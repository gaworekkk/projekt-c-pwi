#include "StatisticsManager.h"
#include <fstream>
#include <iostream>

void StatisticsManager::saveStatistics(int coinCount) {
    std::ofstream file("stats.txt"); 
    if (file.is_open()) {
        file << coinCount << "\n"; 
        file.close();
    }
}

void StatisticsManager::loadStatistics(int& coinCount) {
    std::ifstream file("stats.txt"); 
    if (file.is_open()) {
        file >> coinCount; 
        file.close();
    } else {
        // Jeśli plik nie istnieje ustawiamy domyslne wartosci
        coinCount = 0;
    }
}

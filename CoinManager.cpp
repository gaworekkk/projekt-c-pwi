#include "CoinManager.h"
#include <cstdlib> 
#include <math.h>

CoinManager::CoinManager(float spawnInterval, float speed, float obstacleSpeed)
    : spawnInterval(spawnInterval), speed(speed), obstacleSpawnSpeed(obstacleSpeed) {
    if (!coinTexture.loadFromFile("Tekstury/coin.png")) {
        // Obsłuż błąd ładowania tekstury
    }  
    spawnTimer = 0;
}

void CoinManager::setObstacleSpawnSpeed(float speed) {
    obstacleSpawnSpeed = speed; // Aktualizacja predkosci przeszkod
}

void CoinManager::setSpeed(float newSpeed) {
    speed = newSpeed; // Aktualizacja predkosci przesuwania monet
}

void CoinManager::update(float deltaTime, sf::FloatRect playerBounds, int& coinCount, const std::vector<sf::FloatRect>& obstacleBounds) {
    spawnTimer += deltaTime;

    // Dostosowanie spawnInterval na podstawie obstacleSpawnSpeed
    float adjustedSpawnInterval = spawnInterval - (obstacleSpawnSpeed * 0.05f); 
    if (adjustedSpawnInterval < 1.0f) { // Minimalny czas miedzy monetami
        adjustedSpawnInterval = 1.0f;
    }

    // Generowanie nowej monety
    if (spawnTimer >= adjustedSpawnInterval) {
        bool generate = rand() % 2;
        if (generate) 
        { 
            spawnTimer = 0;
            int maxAttempts = 10; // Mamy 10 prob na wstawienie nowej monety tak aby nie kolidowala z przeszkoda
            int rows = 4; 
            float rowHeight = 50.0f; // Odleglosc miedzy wierszami
            float startY = 375.0f;
            for (int i = 0; i < maxAttempts; ++i) {
                bool isColliding = false;
                int randomRow = rand() % rows;
                float rowY = startY + randomRow * rowHeight;
                sf::Vector2f position(1200, rowY);
                for (const auto& bounds : obstacleBounds) {
                    // Sprawdzenie dystansu między srodkiem monety a przeszkoda
                    sf::Vector2f obstacleCenter(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
                    float dx = position.x - obstacleCenter.x;
                    float dy = position.y - obstacleCenter.y;
                    float distance = std::sqrt(dx * dx + dy * dy);
                    if (distance < 15 + std::max(bounds.width, bounds.height) / 2) {
                        isColliding = true;
                        break;
                    }
                }
                if (!isColliding) {
                    coins.emplace_back(position, 15.0f, coinTexture); // Promien monety = 15
                    break;
                }
            }
        }
        else spawnTimer = 0;
    }

    // Aktualizacja pozycji monet i usuwanie tych ktore wyszly poza ekran
    for (auto it = coins.begin(); it != coins.end();) {
        it->move(-speed * deltaTime, 0);
        bool isCollidingWithObstacle = false;
        for (const auto& bounds : obstacleBounds) {
            if (it->getBounds().intersects(bounds)) {
                isCollidingWithObstacle = true;
                break;
            }
        }
     
        // Usuwamy jesli wyszla poza ekran
        if (it->getPosition().x + it->getBounds().width < 0) {
            it = coins.erase(it);
        }
        // Usuwamy jesli zostala zebrana
        else if (it->getBounds().intersects(playerBounds)) {
            coinCount++;
            it = coins.erase(it);
        }
        else {
            ++it;
        }
    }
}

void CoinManager::draw(sf::RenderWindow& window) {
    for (auto& coin : coins) {
        coin.draw(window);
    }
}

void CoinManager::restart() {
    coins.clear(); 
    setSpeed(initialSpeed);
    spawnTimer = 0;  
}




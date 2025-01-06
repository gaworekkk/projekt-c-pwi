#include "CoinManager.h"
#include <cstdlib> 

CoinManager::CoinManager(float spawnInterval, float speed) {
    this->spawnTimer = 0;             
    this->spawnInterval = spawnInterval; 
    this->speed = speed;   
    // Załaduj teksturę dla monet
    if (!coinTexture.loadFromFile("Tekstury/coin.png")) {
        // Obsłuż błąd ładowania tekstury
    }          
}

void CoinManager::update(float deltaTime, sf::FloatRect playerBounds, int& coinCount, const std::vector<sf::FloatRect>& obstacleBounds) {
    spawnTimer += deltaTime;
    // Generowanie nowej monety
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0;
        int maxAttempts = 5; // Mamy 5 prob na wstawienie nowej monety tak aby nie kolidowala z przeszkoda
        for (int i = 0; i < maxAttempts; ++i) {
            float randomY = static_cast<float>(rand() % 20 + 550);
            sf::Vector2f position(1200, randomY);
            bool isColliding = false;
            for (const auto& bounds : obstacleBounds) {
                if (sf::FloatRect(position.x, position.y, 30, 30).intersects(bounds)) {
                    isColliding = true;
                    break;
                }
            }
            if (!isColliding) {
                coins.emplace_back(position, 10.0f, coinTexture); // Promien monety = 10
                break;
            }
        }
    }
    // Aktualizacja pozycji monet i usuwanie tych ktore wyszły poza ekran
    for (auto it = coins.begin(); it != coins.end();) {
        it->shape.move(-speed * deltaTime, 0); 
        if (it->shape.getPosition().x + it->shape.getRadius() * 2 < 0) {
            it = coins.erase(it);
        }
        else if (it->shape.getGlobalBounds().intersects(playerBounds)) {
            coinCount++;        
            it = coins.erase(it);
        } else {
            ++it;
        }
    }
}

void CoinManager::draw(sf::RenderWindow& window) {
    for (auto& coin : coins) {
        window.draw(coin.shape);
    }
}
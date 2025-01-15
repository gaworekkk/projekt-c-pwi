#ifndef COIN_MANAGER_H
#define COIN_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Coin.h"

class CoinManager {
private:
	bool isSkyLevelOn;
    std::vector<Coin> coins;  
    float spawnTimer;         // Czas od ostatniego wygenerowania monety
    float spawnInterval;      // Czas miedzy generowaniem monet
    float speed;              // Prędkość przesuwania monet
    float initialSpeed;       // Początkowa prędkość generowania monet
    float obstacleSpawnSpeed; // Prędkość generowania przeszkod
    sf::Texture coinTexture; // Tekstura dla monet

public:
	void turnSkyLevelOn();
	void turnSkyLevelOff();
    CoinManager(float spawnInterval, float obstacleSpeed); 
    void update(float deltaTime, sf::FloatRect playerBounds, int& coinCount, const std::vector<sf::FloatRect>& obstacleBounds);
    void draw(sf::RenderWindow& window); 
    void setObstacleSpawnSpeed(float speed);
    void setSpeed(float speed);
    void restart();
};

#endif
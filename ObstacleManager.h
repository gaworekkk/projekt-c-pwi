#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Obstacle.h"
#include <random>
class ObstacleManager {
public:
    ObstacleManager(float windowWidth, float windowHeight, std::string obstacleType);
    void update(float deltaTime);
    void restart();
    void draw(sf::RenderWindow& window);
    void generateObstacle();
    bool checkCollisions(const sf::FloatRect& playerBounds);
    void removeOffscreenObstacles();
    std::vector<sf::FloatRect> getObstacleBounds() const; 

private:
    float screenWidth;
    float screenHeight;
    std::string obstacleType;
    const float groundHeight = 589.f;
    const float skyHeight = 525.f;
    const float birdSpeed = 300.f;
    const float birdWidth = 30.f;
    const float birdHeight = 30.f;
    const float cactusSpeed = 200.f;
    const float cactusWidth = 50.f;
    const float cactusHeight = 50.f;
    std::vector<Obstacle> obstacles;
    float obstacleSpawnTimer;
    float randomSpawnTime;
    std::uniform_real_distribution<float> spawnTimeDistribution; // Zakres losowania czasu
    std::mt19937 rng; // Generator liczb losowych

};

#endif // OBSTACLEMANAGER_H


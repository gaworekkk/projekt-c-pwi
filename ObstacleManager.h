#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Obstacle.h"

class ObstacleManager {
public:
    ObstacleManager(float windowWidth, float windowHeight);
    void update(float deltaTime);
    void restart();
    void draw(sf::RenderWindow& window);
    void generateObstacle();
    bool checkCollisions(const sf::FloatRect& playerBounds);
    void removeOffscreenObstacles();
    std::vector<sf::FloatRect> getObstacleBounds() const; 
    float getRandomSpawnInterval() const;
    void setSpeed(float speed);
    float getSpeed() const;
private:
    float screenWidth;
    float screenHeight;
    const float groundHeight = 600.f;
    std::vector<Obstacle> obstacles;
    float obstacleSpawnTimer;
    float spawnInterval;
    float speed;
};

#endif // OBSTACLEMANAGER_H


#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(float x, float y, float width, float height);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape obstacleShape;
    float speed;
};

#endif // OBSTACLE_H


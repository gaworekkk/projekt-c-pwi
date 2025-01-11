#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(float x, float y, float width, float height, const sf::Texture& texture);
    void update(float deltaTime, float speed);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
private:
    sf::RectangleShape obstacleShape;
};

#endif // OBSTACLE_H


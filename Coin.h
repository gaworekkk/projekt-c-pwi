#ifndef COIN_H
#define COIN_H

#include <SFML/Graphics.hpp>

class Coin {
private:
    sf::CircleShape shape;

public:
    Coin(sf::Vector2f position, float radius);
    void move(float offsetX, float offsetY);
    const sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window);
    const sf::Vector2f& getPosition() const;
};

#endif

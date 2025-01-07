#include "Coin.h"

Coin::Coin(sf::Vector2f position, float radius) {
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
}

void Coin::move(float offsetX, float offsetY) {
    shape.move(offsetX, offsetY);
}

const sf::FloatRect Coin::getBounds() const {
    return shape.getGlobalBounds();
}

void Coin::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

const sf::Vector2f& Coin::getPosition() const {
    return shape.getPosition();
}
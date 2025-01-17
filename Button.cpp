#include "Button.h"
#include <stdexcept>

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const sf::String& text, const sf::Font& font) : isHovered(true) {
    button.setSize(size);
    button.setPosition(position);
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setFillColor(sf::Color::White);

    // Centrowanie tekstu na przycisku
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        position.x + (size.x - textBounds.width) / 2,
        position.y + (size.y - textBounds.height) / 2 - textBounds.top
    );

    normalColor = sf::Color::Blue;
    hoverColor = sf::Color::Red;
    transparentColor = sf::Color(0, 0, 0, 0); // Niebieski z 50% przezroczystością
    button.setFillColor(transparentColor);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(buttonSprite); // Rysowanie sprite
    window.draw(buttonText);
}

void Button::update(const sf::Vector2i& mousePos) {
    if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        if (!isHovered) {
            buttonSprite.setTexture(hoverTexture); // Ustaw teksturę hover
            isHovered = true;
        }
    } else {
        if (isHovered) {
            buttonSprite.setTexture(buttonTexture); // Ustaw normalną teksturę
            isHovered = false;
        }
    }
}

void Button::setText(const std::string text) {
    buttonText.setString(text);
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        button.getPosition().x + (button.getSize().x - textBounds.width) / 2,
        button.getPosition().y + (button.getSize().y - textBounds.height) / 2 - textBounds.top
    );
}

bool Button::isClicked(const sf::Vector2i& mousePos, const sf::Event::MouseButtonEvent& mouseEvent) const {
    return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) &&
           mouseEvent.button == sf::Mouse::Left;
}

std::string Button::getText() const {
    return buttonText.getString(); // Zwraca tekst przycisku jako std::string
}

void Button::setTexture(const std::string& textureFile, const std::string& hoverTextureFile) {
    if (!buttonTexture.loadFromFile(textureFile)) {
        throw std::runtime_error("Nie udało się załadować tekstury: " + textureFile);
    }
    if (!hoverTexture.loadFromFile(hoverTextureFile)) {
        throw std::runtime_error("Nie udało się załadować tekstury hover: " + hoverTextureFile);
    }
    buttonSprite.setTexture(buttonTexture);
    buttonSprite.setPosition(button.getPosition());
    buttonSprite.setScale(
        button.getSize().x / buttonTexture.getSize().x,
        button.getSize().y / buttonTexture.getSize().y
    );
}
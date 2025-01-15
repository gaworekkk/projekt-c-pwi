#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>

// Deklaracja funkcji do wyśrodkowania tekstu
void centerText(sf::Text& text, float windowWidth, float windowHeight, float verticalPosition);
// Funkcja do listy przewijanej
void drawScrollableList(sf::RenderWindow& window, const std::vector<std::wstring>& items, sf::Font& font);

#endif // UTILS_H

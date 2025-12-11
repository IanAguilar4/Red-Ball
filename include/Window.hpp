#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    
    bool isOpen() const;
    void close();
    void clear(sf::Color color = sf::Color::White);
    void display();
    void draw(const sf::Drawable& drawable);
    bool pollEvent(sf::Event& event);
    sf::Vector2u getSize() const;
    
private:
    sf::RenderWindow window;
};

#endif

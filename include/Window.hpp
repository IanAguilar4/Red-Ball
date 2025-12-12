#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    
    bool IsOpen() const;
    void Close();
    void Clear(sf::Color color = sf::Color::White);
    void Display();
    void Draw(const sf::Drawable& drawable);
    bool PollEvent(sf::Event& event);
    sf::Vector2u GetSize() const;
    
private:
    sf::RenderWindow window;
};

#endif

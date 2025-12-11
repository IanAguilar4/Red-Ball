#include <SFML/Graphics.hpp>
#include <iostream>

// Ejemplo: Mostrar texto en pantalla
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Texto");

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        std::cout << "Error: No se pudo cargar la fuente" << std::endl;
        std::cout << "Asegurate de tener una fuente en assets/fonts/" << std::endl;
    }

    // Crear texto del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("RED BALL");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::Red);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(250.f, 100.f);

    // Crear texto de instrucciones
    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setString("Usa las flechas para mover\nPresiona ESPACIO para saltar");
    instructionsText.setCharacterSize(24);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(250.f, 300.f);

    // Crear la bola roja
    sf::CircleShape redBall(50.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setPosition(350.f, 400.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(135, 206, 235));
        window.draw(redBall);
        
        if (font.getInfo().family != "")
        {
            window.draw(titleText);
            window.draw(instructionsText);
        }
        
        window.display();
    }

    return 0;
}

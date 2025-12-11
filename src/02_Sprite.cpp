#include <SFML/Graphics.hpp>
#include <iostream>

// Ejemplo: Usar sprites con animación básica
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Sprite Animado");

    // Crear la bola roja con textura
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/images/redball.png"))
    {
        std::cout << "Error: No se pudo cargar la textura de la bola" << std::endl;
        std::cout << "Usando forma circular por defecto" << std::endl;
    }
    
    sf::Sprite ballSprite(ballTexture);
    ballSprite.setOrigin(25.f, 25.f);
    ballSprite.setPosition(400.f, 300.f);

    // Variables de animación
    float scale = 1.0f;
    float scaleDirection = 0.01f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Animación de escala (efecto de "respiración")
        scale += scaleDirection;
        if (scale > 1.2f || scale < 0.9f)
            scaleDirection = -scaleDirection;
        
        ballSprite.setScale(scale, scale);

        window.clear(sf::Color(135, 206, 235));
        window.draw(ballSprite);
        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>
#include <iostream>

// Ejemplo: Cargar y mostrar una imagen de fondo
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Imagen de Fondo");

    // Cargar una textura de fondo
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/images/background.png"))
    {
        std::cout << "Error: No se pudo cargar la imagen de fondo" << std::endl;
        std::cout << "Usando color de fondo por defecto" << std::endl;
    }
    
    sf::Sprite background(backgroundTexture);

    // Crear la bola roja
    sf::CircleShape redBall(30.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setOrigin(30.f, 30.f);
    redBall.setPosition(400.f, 300.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(135, 206, 235));
        
        // Dibujar fondo si se cargó correctamente
        if (backgroundTexture.getSize().x > 0)
            window.draw(background);
            
        window.draw(redBall);
        window.display();
    }

    return 0;
}

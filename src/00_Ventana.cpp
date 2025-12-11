#include <SFML/Graphics.hpp>

// Ejemplo básico: Crear una ventana simple con SFML
int main()
{
    // Crear una ventana de SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Ventana Basica");

    // Crear una bola roja
    sf::CircleShape redBall(30.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setPosition(385.f, 285.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Verificar si se ha cerrado la ventana
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Limpiar la ventana con color celeste (cielo)
        window.clear(sf::Color(135, 206, 235));

        // Dibujar la bola roja
        window.draw(redBall);

        // Mostrar la ventana
        window.display();
    }

    return 0;
}

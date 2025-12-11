#include <SFML/Graphics.hpp>

// Ejemplo: Controlar la bola con el teclado (básico)
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Control de Teclado");

    // Crear la bola roja
    sf::CircleShape redBall(30.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setOrigin(30.f, 30.f);
    redBall.setPosition(400.f, 300.f);

    float speed = 5.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Control con teclas de dirección
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            redBall.move(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            redBall.move(speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            redBall.move(0, -speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            redBall.move(0, speed);

        window.clear(sf::Color(135, 206, 235));
        window.draw(redBall);
        window.display();
    }

    return 0;
}

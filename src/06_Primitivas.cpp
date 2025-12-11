#include <SFML/Graphics.hpp>

// Ejemplo: Dibujar formas primitivas (plataformas, obstáculos)
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Primitivas");

    // Crear la bola roja
    sf::CircleShape redBall(30.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setOrigin(30.f, 30.f);
    redBall.setPosition(100.f, 100.f);

    // Crear plataformas con rectángulos
    sf::RectangleShape platform1(sf::Vector2f(200.f, 20.f));
    platform1.setPosition(50.f, 300.f);
    platform1.setFillColor(sf::Color(139, 69, 19));

    sf::RectangleShape platform2(sf::Vector2f(150.f, 20.f));
    platform2.setPosition(300.f, 400.f);
    platform2.setFillColor(sf::Color(139, 69, 19));

    sf::RectangleShape platform3(sf::Vector2f(180.f, 20.f));
    platform3.setPosition(500.f, 250.f);
    platform3.setFillColor(sf::Color(139, 69, 19));

    // Crear suelo
    sf::RectangleShape ground(sf::Vector2f(800.f, 40.f));
    ground.setPosition(0.f, 560.f);
    ground.setFillColor(sf::Color(34, 139, 34));

    // Crear obstáculo cuadrado
    sf::RectangleShape obstacle(sf::Vector2f(40.f, 40.f));
    obstacle.setPosition(400.f, 510.f);
    obstacle.setFillColor(sf::Color::Black);

    // Crear un círculo enemigo
    sf::CircleShape enemy(20.f);
    enemy.setFillColor(sf::Color::Blue);
    enemy.setPosition(600.f, 530.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Control simple
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            redBall.move(-3.f, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            redBall.move(3.f, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            redBall.move(0, -3.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            redBall.move(0, 3.f);

        window.clear(sf::Color(135, 206, 235));
        
        // Dibujar todas las formas
        window.draw(ground);
        window.draw(platform1);
        window.draw(platform2);
        window.draw(platform3);
        window.draw(obstacle);
        window.draw(enemy);
        window.draw(redBall);
        
        window.display();
    }

    return 0;
}

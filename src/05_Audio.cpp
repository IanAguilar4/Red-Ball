#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Ejemplo: Reproducir música y efectos de sonido
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Red Ball - Audio");

    // Cargar música de fondo
    sf::Music music;
    if (!music.openFromFile("assets/music/background.ogg"))
    {
        std::cout << "Error: No se pudo cargar la musica de fondo" << std::endl;
    }
    else
    {
        music.setLoop(true);
        music.setVolume(50);
        music.play();
    }

    // Cargar efecto de sonido de salto
    sf::SoundBuffer jumpBuffer;
    if (!jumpBuffer.loadFromFile("assets/music/jump.wav"))
    {
        std::cout << "Error: No se pudo cargar el efecto de sonido de salto" << std::endl;
    }
    sf::Sound jumpSound;
    jumpSound.setBuffer(jumpBuffer);

    // Crear la bola roja
    sf::CircleShape redBall(30.f);
    redBall.setFillColor(sf::Color::Red);
    redBall.setOrigin(30.f, 30.f);
    redBall.setPosition(400.f, 500.f);

    float velocityY = 0.0f;
    float gravity = 0.3f;
    bool onGround = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Salto con efecto de sonido
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space && onGround)
                {
                    velocityY = -15.0f;
                    onGround = false;
                    jumpSound.play();
                }
            }
        }

        // Aplicar gravedad
        velocityY += gravity;
        redBall.move(0, velocityY);

        // Colisión con el suelo
        if (redBall.getPosition().y >= 530.f)
        {
            redBall.setPosition(redBall.getPosition().x, 530.f);
            velocityY = 0;
            onGround = true;
        }

        window.clear(sf::Color(135, 206, 235));
        
        // Dibujar el suelo
        sf::RectangleShape ground(sf::Vector2f(800.f, 40.f));
        ground.setPosition(0.f, 560.f);
        ground.setFillColor(sf::Color(34, 139, 34));
        window.draw(ground);
        
        window.draw(redBall);
        window.display();
    }

    return 0;
}

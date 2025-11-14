#include <SFML/Graphics.hpp>

int main()
{
    sf::VideoMode mode;
    mode.size = { 800, 600 };
    sf::RenderWindow window(mode, "Test SFML 3.0.2");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (true)
        {
            auto eventOpt = window.pollEvent();
            if (!eventOpt.has_value())
                break;

            const sf::Event& event = eventOpt.value();

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        sf::CircleShape circle;
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Green);
        circle.setPosition({ 200.f, 200.f });

        window.draw(circle);
        window.display();
    }

    return 0;
}
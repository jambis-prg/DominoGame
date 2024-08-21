#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <functional>
#include <bit>

#include "UI/UIManager.h"
#include "UI/Elements/UIInteractable.h"

bool running = true;

void ButtonPressed()
{
    std::cout << "O botao foi pressionado\n";
}

void QuitGame()
{
    running = false;
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    UIManager uiManager;
    uiManager.InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), ButtonPressed);
    uiManager.InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), QuitGame);

    
    while (running) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            case sf::Event::MouseButtonPressed:
                {
                    UIMouseButtonPressed e(event.mouseButton.button, sf::Vector2u(event.mouseButton.x, event.mouseButton.y));
                    uiManager.OnEvent(e);
                }
                break;
            case sf::Event::MouseMoved:
                {
                    UIMouseMoved e(sf::Vector2u(event.mouseMove.x, event.mouseMove.y));
                    uiManager.OnEvent(e);
                }
                break;
            default:
                break;
            }
        }

        window.clear();
        uiManager.Draw(window);
        window.display();
    }

    return 0;
}
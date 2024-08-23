#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

#include "UI/UIManager.h"
#include "UI/Elements/UIInteractable.h"
#include "UI/Elements/UIStatic.h"
#include "UI/Font/FontManager.h"
#include "Debug/Log.h"

bool running = true;

UIManager *currentManager = nullptr;
UIManager mainMenu;
UIManager hostMenu;
UIManager clientMenu;
UIText *connectedPlayersText;
int connectedPlayers = 0;

void GoToHostMenu()
{
    currentManager = &hostMenu;
}

void GoToClientMenu()
{
    currentManager = &clientMenu;
}

void QuitGame()
{
    running = false;
}

void BackToMainMenu()
{
    currentManager = &mainMenu;
}

void InsertPlayer()
{
    connectedPlayers++;
    connectedPlayersText->SetText("Connected Players: " + std::to_string(connectedPlayers));
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    if(!FontManager::Init("../resources/font", "arial.ttf")) return 1;
    
    mainMenu.InsertElement<UIText>(sf::Vector2f(400, 100), "Domino\nGame", 30, sf::Color(255, 255, 255));
    mainMenu.InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), GoToHostMenu, "Host", 18);
    mainMenu.InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), GoToClientMenu, "Client", 18);
    mainMenu.InsertElement<UIButton>(sf::Rect<float>(400, 450, 100, 50), QuitGame, "Quit", 18);

    hostMenu.InsertElement<UIText>(sf::Vector2f(400, 100), "Your ip is: 192.168.0.1", 30, sf::Color(255, 255, 255));
    connectedPlayersText = hostMenu.InsertElement<UIText>(sf::Vector2f(400, 155), "Connected Players: 0", 30, sf::Color(255, 255, 255));
    hostMenu.InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), BackToMainMenu, "Back", 18);
    hostMenu.InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), InsertPlayer, "Insert Player", 18);

    std::string ip;
    clientMenu.InsertElement<UITextInput>(sf::Rect<float>(400, 300, 300, 50), "Enter Ip", ip);
    clientMenu.InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), BackToMainMenu, "Back", 18);

    currentManager = &mainMenu;

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
                    currentManager->OnEvent(e);
                }
                break;
            case sf::Event::MouseMoved:
                {
                    UIMouseMoved e(sf::Vector2u(event.mouseMove.x, event.mouseMove.y));
                    currentManager->OnEvent(e);
                }
                break;
            case sf::Event::TextEntered:
                {
                    UIKeyChar e(event.text.unicode);
                    currentManager->OnEvent(e);
                }
            default:
                break;
            }
        }

        window.clear();
        currentManager->Draw(window);
        window.display();
    }

    return 0;
}
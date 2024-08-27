#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <list>

#include "UI/UIManager.h"
#include "UI/Elements/UIInteractable.h"
#include "UI/Elements/UIStatic.h"
#include "UI/Font/FontManager.h"
#include "Debug/Log.h"

#define DEFAULT_PORT 53000
#define MAX_CLIENTS 4
#define TIMEOUT 10.f

#define BIND(method) std::bind(&method, this, std::placeholders::_1)

class Host
{
private:
    sf::TcpListener m_Listener;
    std::list<sf::TcpSocket> m_Clients;
    sf::SocketSelector m_Selector;

    static constexpr float s_SelectorTimeout = 0.1f;
public:
    Host()
    {
        
    }

    ~Host()
    {
        Close();
    }

    void Close()
    {
        for(auto& client : m_Clients)
            client.disconnect();

        m_Listener.close();
        
        m_Selector.clear();
    }

    bool Listen()
    {
        if(m_Listener.listen(DEFAULT_PORT) == sf::Socket::Done)
        {
            m_Selector.add(m_Listener);
            return true;
        }

        return false;
    }

    bool Accept()
    {
        if(m_Clients.size() >= MAX_CLIENTS) return false;

        if(m_Selector.wait(sf::seconds(s_SelectorTimeout)))
        {
            if(m_Selector.isReady(m_Listener))
            {
                // Creating new client
                m_Clients.emplace_back();
                if(m_Listener.accept(m_Clients.back()) == sf::Socket::Done)
                {
                    m_Selector.add(m_Clients.back());
                    return true;
                }
            }
        }

        return false;
    }
};

class Client
{
private:
    sf::TcpSocket m_Socket;
public:
    bool Connect(const std::string &ip)
    {
        return m_Socket.connect(ip, DEFAULT_PORT, sf::seconds(TIMEOUT)) == sf::Socket::Done;
    }
};

class GameManager
{
private:
    enum Menus { MAIN, HOST, CLIENT, FINAL };

    uint32_t m_CurrentMenuID = 0;
    std::vector<UIManager> m_Menus;
    bool m_Running = true, m_Gameplay = false;
    UITextInput *m_IpEntered;

    Host m_Host;
    Client m_Client;

    void ChangeMenu(DataPacket &packet)
    {
        uint32_t newMenu;

        packet >> newMenu;

        // -- Exit --
        switch (m_CurrentMenuID)
        {
        case HOST:
            m_Host.Close();
            break;
        case CLIENT:
            m_IpEntered->SetContent("");
            break;
        default:
            break;
        }

        // -- Enter --
        switch (newMenu)
        {
        case HOST:
            if(!m_Host.Listen())
            {
                LOG_INFO("Nao foi possivel hospedar o server");
                newMenu = m_CurrentMenuID;
            }
            break;
        case CLIENT:
            break;
        default:
            break;
        }

        m_CurrentMenuID = newMenu;
    }

    void QuitGame(DataPacket &packet)
    {
        m_Running = false;
    }
public:
    GameManager() : m_Menus(4)
    {
        DataPacket packet;
        //  -- Main Menu --
        m_Menus[MAIN].InsertElement<UIText>(sf::Vector2f(400, 100), "Domino\nGame", 30, sf::Color(255, 255, 255));
        
        packet << HOST;
        m_Menus[MAIN].InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), BIND(GameManager::ChangeMenu), "Host", 18, packet);

        packet.Clear();
        packet << CLIENT;
        m_Menus[MAIN].InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), BIND(GameManager::ChangeMenu), "Client", 18, packet);
        
        m_Menus[MAIN].InsertElement<UIButton>(sf::Rect<float>(400, 450, 100, 50), BIND(GameManager::QuitGame), "Quit", 18);

        // -- Host Menu --
        packet.Clear();
        packet << MAIN;
        std::string ipText = "   Local ip: " + sf::IpAddress::getLocalAddress().toString() + "\nPublic ip: " + sf::IpAddress::getPublicAddress().toString();
        m_Menus[HOST].InsertElement<UIText>(sf::Vector2f(400, 100), ipText.c_str(), 30, sf::Color(255, 255, 255));
        
        m_Menus[HOST].InsertElement<UIText>(sf::Vector2f(400, 180), "Connected Players: 0", 30, sf::Color(255, 255, 255));
        m_Menus[HOST].InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), BIND(GameManager::ChangeMenu), "Back", 18, packet);

        // -- Client Menu --
        
        m_IpEntered = m_Menus[CLIENT].InsertElement<UITextInput>(sf::Rect<float>(400, 300, 300, 50), "Enter Ip");
        m_Menus[CLIENT].InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), BIND(GameManager::ChangeMenu), "Back", 18, packet);

        // -- Final Menu --
    }

    void OnEvent(sf::Event &event)
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_Running = false;
            break;
        case sf::Event::MouseButtonPressed:
            {
                UIMouseButtonPressed e(event.mouseButton.button, sf::Vector2u(event.mouseButton.x, event.mouseButton.y));
                m_Menus[m_CurrentMenuID].OnEvent(e);
            }
            break;
        case sf::Event::MouseMoved:
            {
                UIMouseMoved e(sf::Vector2u(event.mouseMove.x, event.mouseMove.y));
                m_Menus[m_CurrentMenuID].OnEvent(e);
            }
            break;
        case sf::Event::TextEntered:
            {
                UIKeyChar e(event.text.unicode);
                m_Menus[m_CurrentMenuID].OnEvent(e);
            }
        default:
            break;
        }
    }

    void Update()
    {

    }

    void Draw(sf::RenderWindow &window)
    {
        m_Menus[m_CurrentMenuID].Draw(window);
    }

    bool IsRunning() const { return m_Running; }
};

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    if(!FontManager::Init("../resources/font", "arial.ttf")) return 1;

    GameManager game;

    while (game.IsRunning()) 
    {
        game.Update();

        window.clear();
        game.Draw(window);
        window.display();
        
        sf::Event event;
        while (window.pollEvent(event)) 
            game.OnEvent(event);
    }

    return 0;
}
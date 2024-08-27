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
#define BIND_OVERLOAD(class, method, arg) std::bind(static_cast<void(class::*)(arg)>(&class::method), this, std::placeholders::_1)

enum class SocketOperation { Connect, Disconnect, Last };

sf::Packet& operator<<(sf::Packet& packet, const SocketOperation& operation) {
    // Serializando SocketOperation como um valor inteiro
    packet << static_cast<sf::Uint32>(operation);
    return packet;
}

// Sobrecarga do operador >> para SocketOperation (se precisar desserializar)
sf::Packet& operator>>(sf::Packet& packet, SocketOperation& operation) {
    sf::Uint32 operationInt;
    packet >> operationInt;
    operation = static_cast<SocketOperation>(operationInt);
    return packet;
}

class Host
{
private:
    sf::TcpListener m_Listener;
    std::list<sf::TcpSocket> m_Clients;
    sf::SocketSelector m_Selector;
    std::function<void(SocketOperation)> m_CallbackEvent;
    static constexpr float s_SelectorTimeout = 0.1f;

    void Receive(sf::TcpSocket &client, std::list<sf::TcpSocket>::iterator& it)
    {
        sf::Packet packet;
        SocketOperation operation;
        client.receive(packet);
        packet >> operation;

        switch (operation)
        {
        case SocketOperation::Connect:
            break;
        case SocketOperation::Disconnect:
            client.disconnect();
            it = std::prev(m_Clients.erase(it));
            break;        
        default:
            break;
        }

        m_CallbackEvent(operation);
    }
public:
    Host(std::function<void(SocketOperation)> callbackEvent) : m_CallbackEvent(callbackEvent)
    {
    }

    ~Host()
    {
        Close();
    }

    void Close()
    {
        for(auto& client : m_Clients)
        {
            Send(client, SocketOperation::Disconnect);
            client.disconnect();
        }

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

    void Update()
    {
        if(m_Selector.wait(sf::seconds(s_SelectorTimeout)))
        {
            if(m_Clients.size() < MAX_CLIENTS && m_Selector.isReady(m_Listener))
            {
                // Creating new client
                m_Clients.emplace_back();
                if(m_Listener.accept(m_Clients.back()) == sf::Socket::Done)
                {
                    m_Selector.add(m_Clients.back());
                    m_CallbackEvent(SocketOperation::Connect);
                }
                else
                    m_Clients.pop_back();
            }

            for(auto it = m_Clients.begin(); it != m_Clients.end(); it = std::next(it))
            {
                auto& client = *it;
                if(m_Selector.isReady(client))
                    Receive(client, it);
            }
        }
    }

    template <typename... Args>
    void Send(sf::TcpSocket &socket, SocketOperation operation, Args&&... args)
    {
        sf::Packet packet;
        packet << operation;
        if constexpr (sizeof...(args) > 0)
            (packet << ... << std::forward<Args>(args));
        socket.send(packet);
    }

    uint32_t ClientsCount() const { return m_Clients.size(); }
};

class Client
{
private:
    sf::TcpSocket m_Socket;
    bool m_IsConnected = false;
public:
    bool Connect(const std::string &ip)
    {
        m_IsConnected = m_Socket.connect(ip, DEFAULT_PORT, sf::seconds(TIMEOUT)) == sf::Socket::Done;
        return m_IsConnected;
    }

    void Disconnect()
    {
        Send(SocketOperation::Disconnect);
        m_Socket.disconnect();
        m_IsConnected = false;
    }

    template <typename... Args>
    void Send(SocketOperation operation, Args&&... args)
    {
        if(m_IsConnected)
        {
            sf::Packet packet;
            packet << operation;
            if constexpr(sizeof...(args) > 0)
                (packet << ... << std::forward<Args>(args));
            m_Socket.send(packet);
        }
    }

    void Receive()
    {
        if(m_IsConnected)
        {
            sf::Packet packet;
            m_Socket.receive(packet);
        }
    }
};

class GameManager
{
private:
    enum Menus { MAIN, HOST, CLIENT, LOBBY, FINAL, LAST };

    uint32_t m_CurrentMenuID = 0;
    std::vector<UIManager> m_Menus;
    bool m_Running = true, m_Gameplay = false, m_IsHosting = false;

    UIText *m_ConnectedPlayersText;
    UITextInput *m_IpEntered;
    UIButton *m_HostStartButton;

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
            m_ConnectedPlayersText->SetText("Connected Players: 1");
            m_IsHosting = false;
            break;
        case CLIENT:
            break;
        case LOBBY:
            m_Client.Disconnect();
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
                LOG_DEBUG("Nao foi possivel hospedar o server");
                newMenu = m_CurrentMenuID;
            }
            else
            {
                m_Client.Connect(sf::IpAddress::getLocalAddress().toString());
                m_IsHosting = true;
            }
            break;
        case CLIENT:
            m_IpEntered->SetContent("");
            break;
        case LOBBY:
            if(!m_Client.Connect(m_IpEntered->GetContent()))
            {
                LOG_DEBUG("Nao foi possivel conectar em %s", m_IpEntered->GetContent().c_str());
                newMenu = m_CurrentMenuID;
            }
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
    GameManager() : m_Menus(LAST), m_Host(BIND(GameManager::NetworkEvent))
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
        
        m_ConnectedPlayersText = m_Menus[HOST].InsertElement<UIText>(sf::Vector2f(400, 180), "Connected Players: 1", 30, sf::Color(255, 255, 255));
        m_HostStartButton = m_Menus[HOST].InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), BIND(GameManager::ChangeMenu), "Start", 18, packet);
        m_HostStartButton->SetEnable(false);

        m_Menus[HOST].InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), BIND(GameManager::ChangeMenu), "Back", 18, packet);

        // -- Client Menu --
        
        m_IpEntered = m_Menus[CLIENT].InsertElement<UITextInput>(sf::Rect<float>(400, 300, 300, 50), "Enter Ip");
        m_Menus[CLIENT].InsertElement<UIButton>(sf::Rect<float>(400, 450, 100, 50), BIND(GameManager::ChangeMenu), "Back", 18, packet);

        packet.Clear();
        packet << LOBBY;
        m_Menus[CLIENT].InsertElement<UIButton>(sf::Rect<float>(400, 375, 100, 50), BIND(GameManager::ChangeMenu), "Enter", 18, packet);

        // -- Lobby Menu --
        m_Menus[LOBBY].InsertElement<UIText>(sf::Vector2f(400, 180), "Waiting for others players...", 30, sf::Color(255, 255, 255));
        packet.Clear();
        packet << CLIENT;
        m_Menus[LOBBY].InsertElement<UIButton>(sf::Rect<float>(400, 300, 100, 50), BIND(GameManager::ChangeMenu), "Disconnect", 18, packet);

        // -- Final Menu --
    }

    void NetworkEvent(SocketOperation operation)
    {
        switch (operation)
        {
        case SocketOperation::Connect:
            if(m_Host.ClientsCount() >= MAX_CLIENTS - 1)
                m_HostStartButton->SetEnable(true);
            m_ConnectedPlayersText->SetText("Connected Players: " + std::to_string(m_Host.ClientsCount()));
            break;
        case SocketOperation::Disconnect:
            LOG_DEBUG("Player desconectado");
            m_HostStartButton->SetEnable(false);
            m_ConnectedPlayersText->SetText("Connected Players: " + std::to_string(m_Host.ClientsCount()));
            break;
        default:
            break;
        }
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
        if(!m_Gameplay)
        {
            if(m_CurrentMenuID == HOST)
                m_Host.Update();
        }
        else
        {
            if(m_IsHosting)
                m_Host.Update();

            m_Client.Receive();
        }
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
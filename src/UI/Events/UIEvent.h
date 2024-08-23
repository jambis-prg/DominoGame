#pragma once

#include <SFML/Graphics.hpp>

#define BIT_FLAGS(x) 1 << x

#define EVENT_TYPES_COUNT 4

enum UIEventTypes : uint32_t
{
    MouseButtonPressed = BIT_FLAGS(0),
    MouseScrolled = BIT_FLAGS(1),
    MouseMoved = BIT_FLAGS(2),
    KeyChar = BIT_FLAGS(3),
    All = ~0u
};

class UIEvent
{
public:
    virtual ~UIEvent()
    {
    }

    virtual UIEventTypes GetType() const = 0;
};

class UIMouseButtonPressed : public UIEvent
{
private:
    sf::Mouse::Button m_Button;
    sf::Vector2u m_Position;
public:
    UIMouseButtonPressed(sf::Mouse::Button button, sf::Vector2u position) : m_Button(button), m_Position(position)
    {
    }

    bool LeftPressed() const { return m_Button == sf::Mouse::Button::Left; }
    bool RightPressed() const { return m_Button == sf::Mouse::Button::Right; }
    bool MidPressed() const { return m_Button == sf::Mouse::Button::Middle; }

    const sf::Vector2u& GetPosition() const { return m_Position; }
    uint32_t GetPositionX() const { return m_Position.x; }
    uint32_t GetPositionY() const { return m_Position.y; }

    UIEventTypes GetType() const override
    {
        return UIEventTypes::MouseButtonPressed;
    }
};

class UIMouseMoved : public UIEvent
{
private:
    sf::Vector2u m_Position;
public:
    UIMouseMoved(sf::Vector2u position) :  m_Position(position)
    {
    }

    const sf::Vector2u& GetPosition() const { return m_Position; }
    uint32_t GetPositionX() const { return m_Position.x; }
    uint32_t GetPositionY() const { return m_Position.y; }

    UIEventTypes GetType() const override
    {
        return UIEventTypes::MouseMoved;
    }
};

class UIKeyChar : public UIEvent
{
private:
    uint32_t m_Unicode;
public:
    UIKeyChar(uint32_t unicode) : m_Unicode(unicode)
    {
    }

    uint32_t GetUnicode() const { return m_Unicode; }

    UIEventTypes GetType() const override
    {
        return UIEventTypes::KeyChar;
    }
};
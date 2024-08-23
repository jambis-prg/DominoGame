#pragma once

#include "UIElement.h"
#include "../Font/FontManager.h"

class UIText : public UIElement
{
private:
    sf::Vector2f m_Position;
    sf::Text m_Text;
public:
    UIText(sf::Vector2f position, std::string text, uint32_t size, sf::Color color) : m_Position(position)
    {
        m_Text = sf::Text(text, FontManager::GetDefaultFont(), size);
        sf::Vector2f textBound(m_Text.getGlobalBounds().width, m_Text.getCharacterSize());
        m_Text.setPosition(m_Position - textBound /2.f);
        m_Text.setFillColor(color);
    }

    bool OnEvent(UIEvent &event) override
    {
        return false;
    }

    void Draw(sf::RenderWindow &window) override
    {
        window.draw(m_Text);
    }

    bool IsInteractable() override 
    {
        return false;
    }

    void SetText(std::string text)
    {
        m_Text.setString(text);
    }
};
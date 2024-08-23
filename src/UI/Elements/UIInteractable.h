#pragma once

#include <functional>
#include "UIElement.h"
#include "../Font/FontManager.h"

class UIButton : public UIElement
{
private:
    sf::Vector2f m_Position;
    sf::RectangleShape m_Shape;
    std::function<void(void)> m_Callback;
    bool m_Hovered;
    sf::Color m_Color, m_HoveredColor;
    sf::Text m_Text;
public:

    UIButton(sf::Rect<float> rect, std::function<void(void)> callback, std::string text, uint32_t textSize) : m_Position(rect.getPosition()), m_Shape(rect.getSize()), m_Callback(callback), m_Hovered(false)
    {
        m_Shape.setPosition(m_Position - m_Shape.getSize()/2.f);

        m_Color = m_Shape.getFillColor();
        m_HoveredColor = m_Color;
        m_HoveredColor.r *= 0.75f;
        m_HoveredColor.g *= 0.75f;
        m_HoveredColor.b *= 0.75f;

        m_Text = sf::Text(text,  FontManager::GetDefaultFont(), textSize);
        sf::Vector2f textBound(m_Text.getGlobalBounds().width, m_Text.getCharacterSize());
        m_Text.setPosition(m_Position - textBound /2.f);
        m_Text.setFillColor(sf::Color::Black);

        p_EventFlags = (UIEventTypes)(MouseButtonPressed | MouseMoved);
    }

    ~UIButton()
    {
    }

    bool OnEvent(UIEvent &event) override
    {
        if(m_Callback == nullptr) return false;

        bool handlerResult = false;
        if(DispatchEvent<UIMouseButtonPressed>(event, handlerResult,[this](UIMouseButtonPressed *e)
        {
            auto rect = sf::FloatRect(m_Position, m_Shape.getSize());
            if(e->LeftPressed() && rect.contains((sf::Vector2f)e->GetPosition() + rect.getSize()/2.f))
            {
                m_Callback();
                return true;
            }

            return false;
        }))
            return handlerResult;
        else
        {
            DispatchEvent<UIMouseMoved>(event, handlerResult, [this](UIMouseMoved *e)
            {
                auto rect = sf::FloatRect(m_Position, m_Shape.getSize());

                if(rect.contains((sf::Vector2f)e->GetPosition() + rect.getSize() / 2.f))
                {
                    if(!m_Hovered)
                    {
                        m_Hovered = true;
                        m_Shape.setFillColor(m_HoveredColor);
                    }

                    return true;
                }
                else if(m_Hovered)
                {
                    m_Hovered = false;
                    m_Shape.setFillColor(m_Color);
                }

                return false;
            });
        }

        return false;
    }

    void Draw(sf::RenderWindow &window) override
    {
        window.draw(m_Shape);
        window.draw(m_Text);
    }

    bool IsInteractable() override 
    {
        return true;
    }
};

class UITextInput : public UIElement
{
private:
    sf::Vector2f m_Position;
    sf::RectangleShape m_BackgroundShape;
    sf::Text m_BackgroundText, m_InputText;
    bool m_Selected;
    std::string &m_RefString;
public:
    UITextInput(sf::Rect<float> rect, std::string backgroundText, std::string &refString) : m_Position(rect.getPosition()), m_BackgroundShape(rect.getSize()), m_Selected(false), m_RefString(refString)
    {
        m_BackgroundShape.setPosition(m_Position - m_BackgroundShape.getSize() / 2.f);

        m_BackgroundText = sf::Text(backgroundText, FontManager::GetDefaultFont(), rect.height * 0.6f);
        m_InputText = sf::Text("", FontManager::GetDefaultFont(), rect.height * 0.6f);


        m_BackgroundText.setPosition(m_Position - sf::Vector2f(rect.width, m_InputText.getCharacterSize()) / 2.f);
        m_InputText.setPosition(m_BackgroundText.getPosition());

        m_BackgroundText.setFillColor(sf::Color(150, 150, 150));
        m_InputText.setFillColor(sf::Color::Black);

        p_EventFlags = (UIEventTypes)(MouseButtonPressed | KeyChar);
    }

    ~UITextInput()
    {
    }

    bool OnEvent(UIEvent &event) override
    {
        bool handlerResult = false;
        if(DispatchEvent<UIMouseButtonPressed>(event, handlerResult,[this](UIMouseButtonPressed *e)
        {
            auto rect = sf::FloatRect(m_Position, m_BackgroundShape.getSize());
            if(e->LeftPressed() && rect.contains((sf::Vector2f)e->GetPosition() + rect.getSize()/2.f))
            {
                m_Selected = true;
                return true;
            }

            m_Selected = false;
            return false;
        })) {}
        else
        {
            DispatchEvent<UIKeyChar>(event, handlerResult, [this](UIKeyChar *e)
            {
                if(m_Selected)
                {
                    if(e->GetUnicode() < 0x20)
                    {
                        switch (e->GetUnicode())
                        {
                        case 0x08: // backspace
                            if(!m_RefString.empty())
                                m_RefString.pop_back();
                            break;
                        default:
                            break;
                        }
                    }
                    else
                        m_RefString += e->GetUnicode();
                    m_InputText.setString(m_RefString);
                    return true;
                }

                return false;
            });
        }

        return handlerResult;
    }

    void Draw(sf::RenderWindow &window) override
    {
        window.draw(m_BackgroundShape);

        if(m_RefString.empty() && !m_Selected)
            window.draw(m_BackgroundText);
        
        window.draw(m_InputText);
    }

    bool IsInteractable() override 
    {
        return true;
    }
};
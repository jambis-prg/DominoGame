#pragma once

#include <functional>
#include "UIElement.h"
#include <iostream>


class UIButton : public UIElement
{
private:
    sf::Rect<float> m_Rect;
    sf::RectangleShape m_Shape;
    std::function<void(void)> m_Callback;
    bool m_Hovered;
    sf::Color m_Color, m_HoveredColor;
public:

    UIButton(sf::Rect<float> rect, std::function<void(void)> callback) : m_Rect(rect), m_Callback(callback), m_Hovered(false)
    {
        m_Shape.setPosition(m_Rect.getPosition() - m_Rect.getSize()/2.f);
        m_Shape.setSize(m_Rect.getSize());

        m_Color = m_Shape.getFillColor();
        m_HoveredColor = m_Color;
        m_HoveredColor.r *= 0.75f;
        m_HoveredColor.g *= 0.75f;
        m_HoveredColor.b *= 0.75f;

        p_EventFlags = (UIEventTypes)(MouseButtonPressed | MouseMoved);
    }

    ~UIButton()
    {
    }

    bool OnEvent(UIEvent &event) override
    {
        if(m_Callback == nullptr)
        return false;

        auto e = dynamic_cast<UIMouseButtonPressed*>(&event);
        if(e)
        {
            if(e->LeftPressed() && m_Rect.contains((sf::Vector2f)e->GetPosition() + m_Rect.getSize()/2.f))
            {
                m_Callback();
                return true;
            }
        }
        else
        {
            auto e2 = dynamic_cast<UIMouseMoved*>(&event);

            if(m_Rect.contains((sf::Vector2f)e2->GetPosition() + m_Rect.getSize() / 2.f))
            {
                if(!m_Hovered)
                {
                    m_Hovered = true;
                    m_Shape.setFillColor(m_HoveredColor);
                }
            }
            else if(m_Hovered)
            {
                m_Hovered = false;
                m_Shape.setFillColor(m_Color);
            }
        }

        return false;
    }

    void Draw(sf::RenderWindow &window) override
    {
        window.draw(m_Shape);
    }

    bool IsInteractable() override 
    {
        return true;
    }
};
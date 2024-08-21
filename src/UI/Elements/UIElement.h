#pragma once

#include "../Events/UIEvent.h"

class UIElement
{
protected:
    UIEventTypes p_EventFlags;
public:

    virtual ~UIElement()
    {
    }

    virtual bool OnEvent(UIEvent &event) = 0;
    virtual void Draw(sf::RenderWindow &window) = 0;
    virtual bool IsInteractable() = 0;

    UIEventTypes GetEventFlags() const { return p_EventFlags; }
};
#pragma once

#include "../Events/UIEvent.h"

template<typename T, typename Handler>
bool DispatchEvent(UIEvent &event, bool &handlerResultOut, Handler handler)
{
    auto e = dynamic_cast<T*>(&event);

    if(e)
    {
        handlerResultOut = handler(e);
        return true;
    }

    return false;
}

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
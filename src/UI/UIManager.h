#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

#include "Events/UIEvent.h"
#include "Elements/UIElement.h"

class UIManager
{
private:
    std::vector<UIElement*> m_AllElements;
    std::array<std::vector<uint32_t>, EVENT_TYPES_COUNT> m_EventPools;
public:
    UIManager();
    ~UIManager();

    void OnEvent(UIEvent &event);

    template<typename T, typename... Args>
    void InsertElement(Args&&... args)
    {
        auto *e = new T(std::forward<Args>(args)...);
        m_AllElements.push_back(e);

        if(e->IsInteractable())
        {

            UIEventTypes type = e->GetEventFlags();
            for(uint32_t i = 0; i < EVENT_TYPES_COUNT && type != 0; i++)
            {
                if(type & 1)
                    m_EventPools[i].push_back(m_AllElements.size() - 1); // Salva no Event Pool o indice do elemento

                type = (UIEventTypes)(type >> 1);
            }
        }
    }

    void Draw(sf::RenderWindow &window);
};
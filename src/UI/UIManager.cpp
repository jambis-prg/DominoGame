#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    for(uint32_t i = 0; i < m_AllElements.size(); i++)
        delete m_AllElements[i];
}

void UIManager::OnEvent(UIEvent &event)
{
    uint32_t pos = std::__countr_zero((uint32_t)event.GetType());
    for(uint32_t i = 0; i < m_EventPools[pos].size(); i++)
    {
        if(m_AllElements[m_EventPools[pos][i]]->OnEvent(event))
            return;
    }
}

void UIManager::Draw(sf::RenderWindow &window)
{
    for(auto &element : m_AllElements)
        element->Draw(window);
}
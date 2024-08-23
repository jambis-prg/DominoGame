#include "FontManager.h"

#include "../../Debug/Log.h"
#include "../../Debug/InternalAssert.h"

bool FontManager::s_Initialized = false;
std::string FontManager::s_FontPath = std::string();
sf::Font FontManager::s_DefaultFont = sf::Font();

bool FontManager::Init(const char *fontPath, const char *defaultFont)
{
    ASSERT(!s_Initialized, "Tentativa de inicializar o sistema de fonte novamente");

    s_FontPath = fontPath;

    if(!s_DefaultFont.loadFromFile(s_FontPath + "/" + defaultFont))
    {
        LOG_ERROR("Nao foi possivel carregar a fonte %s", (s_FontPath + "/" + defaultFont).c_str());
        return false;
    }

    s_Initialized = true;
    return true;
}

const sf::Font& FontManager::GetDefaultFont()
{
    ASSERT(s_Initialized, "Nao eh possivel obter a fonte padrao pois o sistema de fonte nao foi inicializado ainda")

    return s_DefaultFont;
}

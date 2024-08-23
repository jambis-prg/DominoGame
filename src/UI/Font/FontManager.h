#pragma once

#include <SFML/Graphics.hpp>

class FontManager
{
private:
    static bool s_Initialized;
    static std::string s_FontPath;
    static sf::Font s_DefaultFont;
public:
    static bool Init(const char *fontsPath, const char *defaultFont);

    static const sf::Font& GetDefaultFont();
};
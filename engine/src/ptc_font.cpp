#include <ptc_font.h>

Font::Font(std::string _path, int _fontSize) : path(_path), fontSize(_fontSize)
{
    for (const auto& [cachePath, cacheFont] : fontCache) {
        if (cachePath == _path) {
        
        }
    }
}

std::map<std::string, Font*> Font::fontCache;
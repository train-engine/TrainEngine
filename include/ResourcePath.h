#ifndef RESOURCEPATH_H_INCLUDED
#define RESOURCEPATH_H_INCLUDED

#include <string>
#include <SFML/Config.hpp>

#if defined(SFML_SYSTEM_MACOS)
    std::string ResourcePath();
#else
    inline std::string ResourcePath()
    {
        return "";
    }
#endif

#endif // RESOURCEPATH_H_INCLUDED

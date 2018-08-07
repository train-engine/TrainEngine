#ifndef MACCLIPBOARD_H_INCLUDED
#define MACCLIPBOARD_H_INCLUDED

#include <SFML/System.hpp>

void SetMacClipboardText(const sf::String& text);
sf::String GetMacClipboardText();

#endif // MACCLIPBOARD_H_INCLUDED

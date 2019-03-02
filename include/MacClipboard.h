#ifndef MACCLIPBOARD_H
#define MACCLIPBOARD_H

#include <SFML/System.hpp>

void SetMacClipboardText(const sf::String& text);
sf::String GetMacClipboardText();

#endif // MACCLIPBOARD_H

#ifndef MACCLIPBOARD_H
#define MACCLIPBOARD_H

#include <SFML/System.hpp>

void setMacClipboardText(const sf::String& text);
sf::String getMacClipboardText();

#endif // MACCLIPBOARD_H

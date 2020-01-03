#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>
#include "Core/Input/InputManager.h"

class TextBox final : public sf::Drawable
{
private:
    sf::RenderTexture m_renderTexture;
    sf::Sprite m_renderSprite;

    InputManager& m_inputManager;

    sf::Vector2f m_position;
    sf::Vector2i m_padding;
    float m_opaquePaddingProportion;
    float m_startingOpacity;
    float m_endingOpacity;

    bool m_hasFocus;
    bool m_isReadOnly;
    bool m_isDigitsOnly;
    bool m_isPasswordModeEnabled;

    unsigned int m_cursorIndex;
    unsigned int m_selectionStartIndex;
    bool m_isCursorVisible;
    unsigned int m_cursorTickCount;
    unsigned int m_cursorBlinkSpeed;
    float m_dragCursorProgress;
    float m_dragCursorSpeedDivider;

    unsigned int m_maxTextLength;
    bool m_textCanExceedBox;

    sf::String m_text;

    sf::Color m_outlineColor;
    sf::Color m_outlineColorFocused;
    sf::Color m_selectionColor;

    sf::Text m_displayText;
    sf::Text m_backgroundText;
    sf::RectangleShape m_box;
    sf::RectangleShape m_cursor;
    sf::RectangleShape m_selection;

    sf::VertexArray m_hideBoxLeft;
    sf::VertexArray m_hideBoxRight;

    // Functions

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void drawTexture();
    bool checkMousePosition() const;
    void centerText();

    void setDisplayText(const sf::String& text);

    // Cursor
    void updateCursor();
    void setCursorPosition();
    void moveCursorRight();
    void moveCursorLeft();
    void controlCursorArrow();
    void controlCursorMouse();
    void dragCursor();
    // Text
    void updateText();
    void setTextPosition();
    void addText(const sf::String& enteredText);
    void deleteSelection();
    // Selection
    void resetSelection();
    void setSelectionBounds();
    void moveCursorToPreviousSpace();
    void moveCursorToNextSpace();

public:
    // Constructor and destructor
    TextBox(InputManager& inputManager, const sf::Font& font);
    TextBox(InputManager& inputManager, const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions);
    ~TextBox();

    // Functions
    void update();
    void handleInput();

    // Setters

    void setMaxTextLength(unsigned int maxTextLength) { m_maxTextLength = maxTextLength; }
    // Position & dimensions
    void setPosition(const sf::Vector2f& position);
    void setDimensions(const sf::Vector2f& dimensions);
    void setPaddingSize(const sf::Vector2i& padding);
    void setOutlineThickness(float outlineThickness) { m_box.setOutlineThickness(outlineThickness); }
    void setCharacterSize(int characterSize);
    void setCursorThickness(float thickness) { m_cursor.setSize(sf::Vector2f(thickness, m_cursor.getSize().y)); }
    // Color
    void setBackgroundColor(sf::Color backgroundColor);
    void setOutlineColorFocused(sf::Color outlineColorFocused) { m_outlineColorFocused = outlineColorFocused; }
    void setOutlineColor(sf::Color outlineColor) { m_outlineColor = outlineColor; }
    void setTextColor(sf::Color textColor) { m_displayText.setFillColor(textColor); }
    void setCursorColor(sf::Color cursorColor) { m_cursor.setFillColor(cursorColor); }
    void setSelectionColor(sf::Color selectionColor) { m_selection.setFillColor(selectionColor); }
    void setBackgroundTextColor(sf::Color textColor) { m_backgroundText.setFillColor(textColor); }
    // Font
    void setFont(const sf::Font& font);

    void setText(const sf::String& text);
    void setPlaceholderText(const std::string& backgroundText) { m_backgroundText.setString(backgroundText); }
    void setFocus(bool hasFocus);
    void setReadOnly(bool isReadOnly) { m_isReadOnly = isReadOnly; }
    void setDigitsOnly(bool isDigitsOnly) { m_isDigitsOnly = isDigitsOnly; }
    void setPasswordModeEnabled(bool isPasswordModeEnabled);

    // Getters
    const sf::Vector2f& getPosition() const { return m_box.getPosition(); }
    const sf::Vector2f& getDimensions() const { return m_box.getSize(); }
    bool hasFocus() const { return m_hasFocus; }
    sf::String getText() const { return m_text; }
};

#endif // TEXTBOX_H

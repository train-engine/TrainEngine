#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>
#include "InputManager.h"

class TextBox final : public sf::Drawable
{
private:
    sf::RenderTexture m_renderTexture;
    sf::Sprite m_renderSprite;

    InputManager& m_rInputManager;

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

    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void DrawTexture();
    bool CheckMousePosition() const;
    void CenterText();

    void SetDisplayText(const sf::String& text);

    // Cursor
    void UpdateCursor();
    void SetCursorPosition();
    void MoveCursorRight();
    void MoveCursorLeft();
    void ControlCursorArrow();
    void ControlCursorMouse();
    void DragCursor();
    // Text
    void UpdateText();
    bool IsCharacterAccepted(sf::Uint32 enteredChar) const;
    void SetTextPosition();
    void AddText(sf::String enteredText);
    void DeleteSelection();
    // Selection
    void ResetSelection();
    void SetSelectionBounds();
    void MoveCursorToPreviousSpace();
    void MoveCursorToNextSpace();

    // Box
    void SetTextSprite();

public:
    // Constructor and destructor
    TextBox(InputManager& rInputManager, const sf::Font& font);
    TextBox(InputManager& rInputManager, const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions);
    ~TextBox();

    // Functions
    void Update();
    void HandleInput();

    // Setters

    void SetMaxTextLength(unsigned int maxTextLength) { m_maxTextLength = maxTextLength; }
    // Position & dimensions
    void SetPosition(const sf::Vector2f& position);
    void SetDimensions(const sf::Vector2f& dimensions);
    void SetPaddingSize(const sf::Vector2i& padding);
    void SetOutlineThickness(float outlineThickness) { m_box.setOutlineThickness(outlineThickness); }
    void SetCharacterSize(int characterSize);
    void SetCursorThickness(float thickness) { m_cursor.setSize(sf::Vector2f(thickness, m_cursor.getSize().y)); }
    // Color
    void SetBackgroundColor(sf::Color backgroundColor);
    void SetOutlineColorFocused(sf::Color outlineColorFocused) { m_outlineColorFocused = outlineColorFocused; }
    void SetOutlineColor(sf::Color outlineColor) { m_outlineColor = outlineColor; }
    void SetTextColor(sf::Color textColor) { m_displayText.setFillColor(textColor); }
    void SetCursorColor(sf::Color cursorColor) { m_cursor.setFillColor(cursorColor); }
    void SetSelectionColor(sf::Color selectionColor) { m_selection.setFillColor(selectionColor); }
    void SetBackgroundTextColor(sf::Color textColor) { m_backgroundText.setFillColor(textColor); }
    // Font
    void SetFont(const sf::Font& font);

    void SetText(const sf::String& text);
    void SetBackgroundText(const std::string& backgroundText) { m_backgroundText.setString(backgroundText); }
    void SetFocus(bool hasFocus);
    void SetReadOnly(bool isReadOnly) { m_isReadOnly = isReadOnly; }
    void SetDigitsOnly(bool isDigitsOnly) { m_isDigitsOnly = isDigitsOnly; }
    void SetPasswordModeEnabled(bool isPasswordModeEnabled);

    // Getters
    const sf::Vector2f& GetPosition() const { return m_box.getPosition(); }
    const sf::Vector2f& GetDimensions() const { return m_box.getSize(); }
    bool HasFocus() const { return m_hasFocus; }
    sf::String GetText() const { return m_text; }
};

#endif // TEXTBOX_H

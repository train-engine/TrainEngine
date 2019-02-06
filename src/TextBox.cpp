#include "TextBox.h"
#include <cmath>

namespace
{
    const sf::Uint32 passwordChar = 8226;
}

TextBox::TextBox(InputManager& rInputManager, const sf::Font& font)
    : TextBox(rInputManager, font, sf::Vector2f(0, 0), sf::Vector2f(100, 20))
{

}

TextBox::TextBox(InputManager& rInputManager, const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions)
    :  m_rInputManager(rInputManager),
       m_position(position),
       m_padding({6, 4}),
       m_opaquePaddingProportion(0.25),
       m_startingOpacity(64),
       m_endingOpacity(255),
       m_hasFocus(false),
       m_isReadOnly(false),
       m_isDigitsOnly(false),
       m_isPasswordModeEnabled(false),
       m_cursorIndex(0),
       m_selectionStartIndex(0),
       m_isCursorVisible(true),
       m_cursorTickCount(0),
       m_cursorBlinkSpeed(30),
       m_dragCursorProgress(0),
       m_dragCursorSpeedDivider(20),
       m_maxTextLength(256),
       m_textCanExceedBox(true),
       m_text(""),
       m_hideBoxLeft(sf::TriangleStrip, 6),
       m_hideBoxRight(sf::TriangleStrip, 6)
{
    // Background color
    SetBackgroundColor(sf::Color::White);

    // Outline settings
    SetOutlineThickness(1);
    SetOutlineColor(sf::Color(90, 90, 90));
    SetOutlineColorFocused(sf::Color(36, 109, 226));
    if (m_hasFocus == true) m_box.setOutlineColor(m_outlineColorFocused);
    else m_box.setOutlineColor(m_outlineColor);

    // Text settings
    SetText("");
    SetBackgroundText("");
    SetFont(font);
    SetTextColor(sf::Color::Black);
    SetBackgroundTextColor(sf::Color(90, 90, 90));
    SetCharacterSize(30);

    // Cursor settings
    SetCursorColor(sf::Color::Black);
    SetCursorThickness(1);

    // Selection settings
    SetSelectionColor(sf::Color(36, 109, 226, 150));

    SetDimensions(dimensions);
    SetPosition(m_position);

    m_renderTexture.clear();
}

TextBox::~TextBox()
{
}

void TextBox::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
    rTarget.draw(m_box, states);
    if (m_cursorIndex != m_selectionStartIndex)
    {
        rTarget.draw(m_selection, states);
    }
    rTarget.draw(m_renderSprite, states);
    rTarget.draw(m_hideBoxLeft, states);
    rTarget.draw(m_hideBoxRight, states);
    if (m_hasFocus == true && m_isCursorVisible == true)
    {
        rTarget.draw(m_cursor, states);
    }
    if (m_hasFocus == false && m_displayText.getString().isEmpty())
    {
        rTarget.draw(m_backgroundText, states);
    }
}

void TextBox::DrawTexture()
{
    m_renderTexture.clear(sf::Color::Transparent);
    m_renderTexture.draw(m_displayText);
    m_renderTexture.display();
}

// Check if the mouse is inside the box
bool TextBox::CheckMousePosition() const
{
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(m_rInputManager.GetWindowMousePosition());
    return (mousePos.x > m_box.getPosition().x && mousePos.x < m_box.getPosition().x + m_box.getSize().x &&
            mousePos.y > m_box.getPosition().y && mousePos.y < m_box.getPosition().y + m_box.getSize().y);
}

void TextBox::CenterText()
{
    m_displayText.setPosition(m_displayText.getPosition().x, std::round(m_position.y + (m_box.getSize().y - m_displayText.getFont()->getLineSpacing(m_displayText.getCharacterSize())) / 2));
    m_backgroundText.setPosition(m_box.getPosition().x + m_padding.x, m_displayText.getPosition().y);
}


void TextBox::SetDisplayText(const sf::String& text)
{
    m_text = text;
    if (m_isPasswordModeEnabled == true)
    {
        sf::String passwordDisplayText = m_text;
        for (auto& rChar : passwordDisplayText)
        {
            rChar = passwordChar;
        }
        m_displayText.setString(passwordDisplayText);
    }
    else
    {
        m_displayText.setString(text);
    }
}

void TextBox::UpdateCursor()
{
    // If the left or right arrows are pressed
    if (m_rInputManager.IsKeyDescending(sf::Keyboard::Left, true) || m_rInputManager.IsKeyDescending(sf::Keyboard::Right, true))
    {
        ControlCursorArrow();
        SetTextPosition();
        DrawTexture();
        SetCursorPosition();
        if (m_selectionStartIndex != m_cursorIndex)
        {
            SetSelectionBounds();
        }
    }
    if (m_rInputManager.IsMouseButtonHeld(sf::Mouse::Left) || m_rInputManager.IsMouseButtonDescending(sf::Mouse::Left))
    {
        if (m_rInputManager.GetWindowMousePosition().x > m_box.getPosition().x &&
            m_rInputManager.GetWindowMousePosition().x < m_box.getPosition().x + m_box.getSize().x)
        {
            if ((m_rInputManager.IsMouseButtonHeld(sf::Mouse::Left) && m_rInputManager.DetectedMouseMovedEvent()) ||
                 m_rInputManager.IsMouseButtonDescending(sf::Mouse::Left))
            {
                ControlCursorMouse();
            }
        }
        else
        {
            DragCursor();
        }
        SetTextPosition();
        DrawTexture();
        SetCursorPosition();
        // Reset selection on mouse click
        if (m_rInputManager.IsMouseButtonDescending(sf::Mouse::Left))
        {
            ResetSelection();
        }
        else
        {
            SetSelectionBounds();
        }
    }
}

// Set the position of the cursor at its character position
void TextBox::SetCursorPosition()
{
    if (m_displayText.getString().isEmpty())
    {
        m_cursor.setPosition(sf::Vector2f(m_box.getPosition().x + m_padding.x, m_cursor.getPosition().y));
    }
    else
    {
        m_cursor.setPosition(m_displayText.findCharacterPos(m_cursorIndex).x, m_cursor.getPosition().y);
    }
}

void TextBox::MoveCursorRight()
{
    m_cursorIndex++;
    m_isCursorVisible = true;
    m_cursorTickCount = 0;
}

void TextBox::MoveCursorLeft()
{
    m_cursorIndex--;
    m_isCursorVisible = true;
    m_cursorTickCount = 0;
}

// Control left and right arrows
// Shift selection is supported
void TextBox::ControlCursorArrow()
{
    // If left arrow key is pressed
    if (m_rInputManager.IsKeyDescending(sf::Keyboard::Left, true))
    {
        // If there is a selection and shift and control are not held
        if (m_selectionStartIndex != m_cursorIndex && !m_rInputManager.IsShiftKeyHeld())
        {
            if (m_selectionStartIndex < m_cursorIndex)
            {
                m_cursorIndex = m_selectionStartIndex;
            }
            else
            {
                m_selectionStartIndex = m_cursorIndex;
            }
            m_isCursorVisible = true;
            m_cursorTickCount = 0;
        }
        // If there is no selection and the cursor has space to go left
        else if (m_cursorIndex > 0)
        {
            // Modifier key
            if (m_rInputManager.IsModifierKeyHeld())
            {
                // Loop through text
                MoveCursorToPreviousSpace();
            }
            else
            {
                MoveCursorLeft();
            }
            if (!m_rInputManager.IsShiftKeyHeld())
            {
                m_selectionStartIndex = m_cursorIndex;
            }
        }
    }
    // If the right arrow key is pressed
    else if (m_rInputManager.IsKeyDescending(sf::Keyboard::Right, true))
    {
        // If there is a selection and shift and control are not held
        if (m_selectionStartIndex != m_cursorIndex && !m_rInputManager.IsShiftKeyHeld())
        {
            if (m_selectionStartIndex < m_cursorIndex)
            {
                m_selectionStartIndex = m_cursorIndex;
            }
            else
            {
                m_cursorIndex = m_selectionStartIndex;
            }
            m_isCursorVisible = true;
            m_cursorTickCount = 0;
        }
        // If there is no selection and the cursor has space to go right
        else if (m_cursorIndex < m_displayText.getString().getSize())
        {
            // Modifier key
            if (m_rInputManager.IsModifierKeyHeld())
            {
                MoveCursorToNextSpace();
            }
            else
            {
                MoveCursorRight();
            }
            if (!m_rInputManager.IsShiftKeyHeld())
            {
                m_selectionStartIndex = m_cursorIndex;
            }
        }
    }
}

// Control the cursor with the mouse
void TextBox::ControlCursorMouse()
{
    m_cursorTickCount = 0;
    m_isCursorVisible = true;

    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(m_rInputManager.GetWindowMousePosition());
    sf::Vector2f cursorPosition = m_cursor.getPosition();
    if (mousePosition.x >= m_displayText.getPosition().x + m_displayText.getLocalBounds().width)
    {
        m_cursorIndex = static_cast<unsigned int>(m_displayText.getString().getSize());
        return;
    }
    else if (mousePosition.x <= m_displayText.getPosition().x)
    {
        m_cursorIndex = 0;
        return;
    }

    if (mousePosition.x > cursorPosition.x)
    {
        while ((m_displayText.findCharacterPos(m_cursorIndex).x +  m_displayText.findCharacterPos(m_cursorIndex + 1).x) / 2 < mousePosition.x)
        {
            if (m_cursorIndex >= m_displayText.getString().getSize())
            {
                break;
            }
            m_cursorIndex++;
        }
    }
    else if (mousePosition.x < cursorPosition.x)
    {
        while ((m_displayText.findCharacterPos(m_cursorIndex).x +  m_displayText.findCharacterPos(m_cursorIndex - 1).x) / 2 >= mousePosition.x)
        {
            m_cursorIndex--;
            if (m_cursorIndex <= 0)
            {
                break;
            }
        }
    }
}

void TextBox::DragCursor()
{
    float mouseDistance = 0;
    if (m_rInputManager.GetWindowMousePosition().x <= m_box.getPosition().x)
    {
        if (m_cursorIndex > 0) mouseDistance = m_rInputManager.GetWindowMousePosition().x - m_box.getPosition().x;
    }
    else if (m_cursorIndex < m_displayText.getString().getSize())
    {
        mouseDistance = m_rInputManager.GetWindowMousePosition().x - (m_box.getSize().x + m_box.getPosition().x);
    }

    m_dragCursorProgress += mouseDistance / m_dragCursorSpeedDivider;
    bool cursorMoved = false;

    while (m_dragCursorProgress <= -1 && m_cursorIndex > 0)
    {
        m_dragCursorProgress++;
        MoveCursorLeft();
        cursorMoved = true;
    }
    while (m_dragCursorProgress >= 1 && m_cursorIndex < m_displayText.getString().getSize())
    {
        m_dragCursorProgress--;
        MoveCursorRight();
        cursorMoved = true;
    }

    if (cursorMoved == true)
    {
        SetTextPosition();
        DrawTexture();
        SetCursorPosition();
        SetSelectionBounds();
    }
}

void TextBox::UpdateText()
{
    if (m_rInputManager.DetectedTextEnteredEvent() || m_rInputManager.IsKeyDescending(sf::Keyboard::Delete, true))
    {
        m_isCursorVisible = true;
        m_cursorTickCount = 0;
        for (const auto& enteredChar : m_rInputManager.GetEnteredText())
        {
            // If the text entered is not an undesirable character
            if ((m_isDigitsOnly == false && (IsCharacterAccepted(enteredChar) ||
                enteredChar == 3 ||     // CTRL + C
                enteredChar == 22 ||    // CTRL + V
                enteredChar == 24)) ||  // CTRL + X
                (m_isDigitsOnly == true && enteredChar >= 48 && enteredChar <= 57))

            {
                if (enteredChar == 3 || enteredChar == 24) // CTRL + C or CTRL + X
                {
                    // If there is a selection
                    if (m_selectionStartIndex != m_cursorIndex)
                    {
                        sf::String selectedText;
                        if (m_selectionStartIndex < m_cursorIndex)
                        {
                            selectedText = m_text.substring(m_selectionStartIndex, m_cursorIndex - m_selectionStartIndex);
                        }
                        else
                        {
                            selectedText = m_text.substring(m_cursorIndex, m_selectionStartIndex - m_cursorIndex);
                        }
                        m_rInputManager.SetClipboardText(selectedText);
                        if (enteredChar == 24)
                        {
                            DeleteSelection();
                            if (m_cursorIndex > m_selectionStartIndex) m_cursorIndex = m_selectionStartIndex;
                            else m_selectionStartIndex = m_cursorIndex;

                            SetSelectionBounds();
                        }
                    }
                }
                else if (enteredChar == 22) // CTRL + V
                {
                    // Add all clipboard characters
                    for (const auto& clipboardChar : m_rInputManager.GetClipboardText())
                    {
                        sf::String pastedText;
                        if (IsCharacterAccepted(clipboardChar) || clipboardChar == '\n')
                        {
                            if (clipboardChar == '\n') pastedText += ' ';
                            else pastedText += clipboardChar;
                            AddText(pastedText);
                            m_selectionStartIndex = m_cursorIndex;
                        }
                    }
                }
                else
                {
                    // If there is a selection
                    if (m_selectionStartIndex != m_cursorIndex && enteredChar != 3 && enteredChar != 24)
                    {
                        // Delete the selection before adding a character
                        DeleteSelection();

                        if (m_cursorIndex > m_selectionStartIndex) m_cursorIndex = m_selectionStartIndex;
                        else m_selectionStartIndex = m_cursorIndex;

                        SetSelectionBounds();
                    }
                    AddText(enteredChar);
                    if (m_textCanExceedBox == false && m_displayText.getLocalBounds().left + m_displayText.getLocalBounds().width > m_box.getSize().x - m_padding.x * 2)
                    {
                        sf::String text = m_displayText.getString();
                        text.erase(m_cursorIndex - 1);
                        m_cursorIndex--;
                        SetDisplayText(text);
                    }
                }
                SetTextPosition();
                DrawTexture();
                SetCursorPosition();
                if (enteredChar != 3)
                {
                    m_selectionStartIndex = m_cursorIndex;
                }
            }
            // If the text is a delete key
            else if (enteredChar == '\b' || enteredChar == 127)
            {
                if (m_displayText.getString().getSize() > 0)
                {
                        if (m_rInputManager.IsModifierKeyHeld())
                    {
                        // If CTRL + backspace, delete whole section
                        if (enteredChar == '\b')
                        {
                            MoveCursorToPreviousSpace();
                        }
                        else
                        {
                            MoveCursorToNextSpace();
                        }

                    }
                    // If there is a selection
                    if (m_selectionStartIndex != m_cursorIndex)
                    {
                        DeleteSelection();

                        if (m_cursorIndex > m_selectionStartIndex) m_cursorIndex = m_selectionStartIndex;
                        else m_selectionStartIndex = m_cursorIndex;

                        SetSelectionBounds();
                    }
                    else
                    {
                        sf::String text = m_text;
                        if (enteredChar == '\b')
                        {
                            if (m_cursorIndex > 0)
                            {
                                if (m_rInputManager.IsShiftKeyHeld())
                                {
                                    m_cursorIndex = 0;
                                    DeleteSelection();
                                }
                                else
                                {
                                    text.erase(m_cursorIndex - 1);
                                    m_cursorIndex--;
                                    SetDisplayText(text);
                                }
                            }
                        }
                        else if (enteredChar == 127)
                        {
                            MoveCursorToPreviousSpace();
                            DeleteSelection();
                        }
                    }
                    SetTextPosition();
                    DrawTexture();
                    SetCursorPosition();
                }
                m_selectionStartIndex = m_cursorIndex;
            }
            else if (enteredChar == 1) // CTRL + A
            {
                m_selectionStartIndex = 0;
                m_cursorIndex = static_cast<unsigned int>(m_displayText.getString().getSize());
                SetTextPosition();
                DrawTexture();
                SetSelectionBounds();
                SetCursorPosition();
            }
        }
        if (m_rInputManager.IsKeyDescending(sf::Keyboard::Delete, true))
        {
            if (m_cursorIndex < m_displayText.getString().getSize())
            {
                sf::String text = m_displayText.getString();
                if (m_rInputManager.IsModifierKeyHeld())
                {
                    MoveCursorToNextSpace();
                    DeleteSelection();
                    m_selectionStartIndex = m_cursorIndex;
                    SetTextPosition();
                    SetCursorPosition();
                }
                else
                {
                    text.erase(m_cursorIndex);
                    SetDisplayText(text);
                    SetTextPosition();
                }
                DrawTexture();
            }

        }
    }
}

bool TextBox::IsCharacterAccepted(sf::Uint32 enteredChar)
{
    return ((enteredChar >= 32 && enteredChar <= 126) || enteredChar > 160);
}

// Set the position of the text
void TextBox::SetTextPosition()
{
    // If the text is bigger than the box
    if (m_displayText.getLocalBounds().left + m_displayText.getLocalBounds().width >= m_box.getSize().x - 2 * m_padding.x)
    {
        // If there is a gap between the right side of the box and the text, move the text
        if (m_displayText.getGlobalBounds().left + m_displayText.getLocalBounds().left + m_displayText.getLocalBounds().width < m_box.getPosition().x + m_box.getSize().x - m_padding.x)
        {
            m_displayText.move(m_box.getPosition().x + m_box.getSize().x - m_padding.x - (m_displayText.getGlobalBounds().left + m_displayText.getLocalBounds().left + m_displayText.getLocalBounds().width), 0);
        }

        // If the cursor's character is out of bounds, move the text
        if (m_displayText.findCharacterPos(m_cursorIndex).x - m_displayText.getLocalBounds().left > m_box.getPosition().x + m_box.getSize().x - m_padding.x)
        {
            m_displayText.move(m_box.getPosition().x + m_box.getSize().x - m_padding.x - m_displayText.findCharacterPos(m_cursorIndex).x + m_displayText.getLocalBounds().left, 0);
        }
        else if (m_displayText.findCharacterPos(m_cursorIndex).x < m_box.getPosition().x + m_padding.x)
        {
            m_displayText.move(m_box.getPosition().x + m_padding.x - m_displayText.findCharacterPos(m_cursorIndex).x, 0);
        }
    }
    // If the text is smaller than the box and is not on the left of the box
    else if (m_displayText.getPosition().x != m_box.getPosition().x + m_padding.x)
    {
        m_displayText.move(m_box.getPosition().x + m_padding.x - m_displayText.getPosition().x, 0);
    }
}

void TextBox::AddText(sf::String enteredText)
{
    // Check if text is too big
    if (m_displayText.getString().getSize() < m_maxTextLength || m_maxTextLength == 0)
    {
        // Add the character where the cursor is
        sf::String text = m_text;
        if (m_cursorIndex != m_selectionStartIndex)
        {
            if (m_cursorIndex < m_selectionStartIndex)
            {
                text.erase(m_cursorIndex, m_selectionStartIndex - m_cursorIndex);
            }
            else
            {
                text.erase(m_selectionStartIndex, m_cursorIndex - m_selectionStartIndex);
                m_cursorIndex -= (m_cursorIndex - m_selectionStartIndex);
            }
        }
        text.insert(m_cursorIndex, enteredText);
        SetDisplayText(text);
        m_cursorIndex++;
    }
}

void TextBox::DeleteSelection()
{
    sf::String text = m_displayText.getString();
    if (m_cursorIndex < m_selectionStartIndex)
    {
        text.erase(m_cursorIndex, m_selectionStartIndex - m_cursorIndex);
    }
    else
    {
        text.erase(m_selectionStartIndex, m_cursorIndex - m_selectionStartIndex);
        m_cursorIndex -= (m_cursorIndex - m_selectionStartIndex);
    }
    SetText(text);
}

void TextBox::ResetSelection()
{
    m_selectionStartIndex = m_cursorIndex;
    if (m_selection.getSize().x > 0)
    {
        SetSelectionBounds();
    }
}

// Set the position and size of the selection box
void TextBox::SetSelectionBounds()
{
    sf::Vector2f position = sf::Vector2f(m_displayText.findCharacterPos(m_selectionStartIndex).x, m_selection.getPosition().y);
    sf::Vector2f dimensions = sf::Vector2f(m_displayText.findCharacterPos(m_cursorIndex).x - m_displayText.findCharacterPos(m_selectionStartIndex).x, m_selection.getSize().y);
    if (dimensions.x < 0)
    {
        position.x += dimensions.x;
        dimensions.x = -dimensions.x;
    }

    if (position.x < m_box.getPosition().x)
    {
        dimensions.x -= (m_box.getPosition().x - position.x);
        position.x = m_box.getPosition().x;
    }

    if (position.x + dimensions.x > m_box.getPosition().x + m_box.getSize().x)
    {
        dimensions.x = m_position.x + m_box.getSize().x - position.x;
    }
    m_selection.setPosition(position);
    m_selection.setSize(dimensions);
}

void TextBox::MoveCursorToPreviousSpace()
{
    while (m_cursorIndex > 0)
    {
        // Move cursor
        MoveCursorLeft();
        // Until there is a space
        if (m_cursorIndex > 1 && m_displayText.getString()[m_cursorIndex - 1] == ' ')
        {
            break;
        }
    }
}

void TextBox::MoveCursorToNextSpace()
{
    while (m_cursorIndex < m_displayText.getString().getSize())
    {
        // Move the cursor
        MoveCursorRight();
        // Until there is a space
        if (m_cursorIndex < m_displayText.getString().getSize() && m_displayText.getString()[m_cursorIndex] == ' ')
        {
            break;
        }
    }
}

void TextBox::Update()
{
    if (m_isReadOnly == false)
    {
        // Update cursor blinking
        m_cursorTickCount++;
        if (m_cursorTickCount >= m_cursorBlinkSpeed)
        {
            m_isCursorVisible = (!m_isCursorVisible);
            m_cursorTickCount = 0;
        }
    }
}

void TextBox::HandleInput()
{
    if (m_isReadOnly == false)
    {
        if (m_hasFocus == true)
        {
            UpdateText();
            UpdateCursor();
        }

        // Check if there is a mouse press
        if (m_rInputManager.IsMouseButtonDescending(sf::Mouse::Left))
        {
            bool isMouseInsideBox = CheckMousePosition();
            if (isMouseInsideBox == true && m_hasFocus == false)
            {
                m_hasFocus = true;
                ControlCursorMouse();
                SetCursorPosition();
                m_box.setOutlineColor(m_outlineColorFocused);
                ResetSelection();
            }
            else if (isMouseInsideBox == false && m_hasFocus == true)
            {
                m_hasFocus = false;
                m_box.setOutlineColor(m_outlineColor);
                ResetSelection();
            }
        }

        // Check if the window lost focus
        if (m_rInputManager.DetectedLostFocusEvent())
        {
            m_hasFocus = false;
            m_box.setOutlineColor(m_outlineColor);
            ResetSelection();
        }
    }
}

void TextBox::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_box.setPosition(position);
    CenterText();
    m_cursor.setPosition(position.x + m_padding.x, m_displayText.getPosition().y);
    m_selection.setPosition({position.x, m_cursor.getPosition().y});
    m_renderTexture.setView(sf::View(sf::FloatRect(position.x, position.y, m_box.getSize().x, m_box.getSize().y)));
    m_renderSprite.setPosition(position);
    SetPaddingSize(m_padding);
}

void TextBox::SetDimensions(const sf::Vector2f& dimensions)
{
    if (dimensions.x == 0 || dimensions.y == 0)
    {
        return;
    }
    m_box.setSize(dimensions);
    CenterText();
    m_cursor.setPosition(m_position.x + m_padding.x, m_displayText.getPosition().y);
    m_selection.setPosition({m_box.getPosition().x, m_cursor.getPosition().y});
    SetPaddingSize(m_padding);
    m_renderTexture.create(dimensions.x, dimensions.y);
    m_renderTexture.setView(sf::View(sf::FloatRect(m_box.getPosition().x, m_box.getPosition().y, dimensions.x, dimensions.y)));
    m_renderSprite.setTexture(m_renderTexture.getTexture(), true);
    SetPaddingSize(m_padding);
    DrawTexture();
}

void TextBox::SetPaddingSize(const sf::Vector2i& padding)
{
    m_padding = padding;
    m_hideBoxLeft[0].position = m_position;
    m_hideBoxLeft[1].position = m_position + sf::Vector2f(0, m_box.getSize().y);
    m_hideBoxLeft[2].position = m_position + sf::Vector2f(m_padding.x * m_opaquePaddingProportion, 0);
    m_hideBoxLeft[3].position = m_position + sf::Vector2f(m_padding.x * m_opaquePaddingProportion, m_box.getSize().y);
    m_hideBoxLeft[4].position = m_position + sf::Vector2f(m_padding.x, 0);
    m_hideBoxLeft[5].position = m_position + sf::Vector2f(m_padding.x, m_box.getSize().y);
    m_hideBoxRight[0].position = m_position + sf::Vector2f(m_box.getSize().x, 0);
    m_hideBoxRight[1].position = m_position + m_box.getSize();
    m_hideBoxRight[2].position = m_position + sf::Vector2f(m_box.getSize().x - (m_padding.x * m_opaquePaddingProportion), 0);
    m_hideBoxRight[3].position = m_position + sf::Vector2f(m_box.getSize().x - (m_padding.x * m_opaquePaddingProportion), m_box.getSize().y);
    m_hideBoxRight[4].position = m_position + sf::Vector2f(m_box.getSize().x - (m_padding.x), 0);
    m_hideBoxRight[5].position = m_position + sf::Vector2f(m_box.getSize().x - (m_padding.x), m_box.getSize().y);
}

void TextBox::SetCharacterSize(int characterSize)
{
    m_displayText.setCharacterSize(characterSize);
    m_backgroundText.setCharacterSize(characterSize);
    m_cursor.setSize(sf::Vector2f(m_cursor.getSize().x, m_displayText.getFont()->getLineSpacing(m_displayText.getCharacterSize())));
    m_selection.setSize({m_selection.getSize().x, m_cursor.getSize().y});
    CenterText();
}

void TextBox::SetBackgroundColor(sf::Color backgroundColor)
{
    m_box.setFillColor(backgroundColor);
    sf::Color boxColorFade = backgroundColor;
    boxColorFade.a = m_startingOpacity;
    backgroundColor.a = m_endingOpacity;
    m_hideBoxLeft[0].color = backgroundColor;
    m_hideBoxLeft[1].color = backgroundColor;
    m_hideBoxLeft[2].color = backgroundColor;
    m_hideBoxLeft[3].color = backgroundColor;
    m_hideBoxLeft[4].color = boxColorFade;
    m_hideBoxLeft[5].color = boxColorFade;
    m_hideBoxRight[0].color = backgroundColor;
    m_hideBoxRight[1].color = backgroundColor;
    m_hideBoxRight[2].color = backgroundColor;
    m_hideBoxRight[3].color = backgroundColor;
    m_hideBoxRight[4].color = boxColorFade;
    m_hideBoxRight[5].color = boxColorFade;
}

void TextBox::SetFont(const sf::Font& font)
{
    m_displayText.setFont(font);
    m_backgroundText.setFont(font);
    CenterText();
}

void TextBox::SetText(const sf::String& text)
{
    SetDisplayText(text);
    SetTextPosition();
    m_selectionStartIndex = m_cursorIndex;
    SetCursorPosition();
    DrawTexture();
}

void TextBox::SetFocus(bool hasFocus)
{
    if (m_hasFocus != hasFocus)
    {
        m_hasFocus = hasFocus;
        if (m_hasFocus == true)
        {
            m_box.setOutlineColor(m_outlineColorFocused);
        }
        else
        {
            m_box.setOutlineColor(m_outlineColor);
        }
    }
}

void TextBox::SetPasswordModeEnabled(bool isPasswordModeEnabled)
{
    m_isPasswordModeEnabled = isPasswordModeEnabled;
    SetDisplayText(m_text);
}

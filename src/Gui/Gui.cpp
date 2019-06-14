#include "Gui/Gui.h"
#include <cmath>

// DEPRECATED

// Base InteractiveGui

InteractiveGui::InteractiveGui()
    : m_state(GuiState::Normal)
    , m_isClicked(false)
{
}

// GuiRectButton

GuiRectButton::GuiRectButton(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions, float borderThickness,
                             float textPadding, const std::string& text, GuiStyle style)
    : m_borderThickness(borderThickness)
    , m_textPadding(textPadding)
{
    setStyle(style);
    setFont(font);
    setText(text);
    setDimensions(dimensions);
    setPosition(position);
    m_shape.setOutlineThickness(m_borderThickness);
    m_shape.setFillColor(m_bodyColor);
    m_shape.setOutlineColor(m_borderColor);
    m_text.setFillColor(m_textColor);
}

void GuiRectButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_text, states);
}

bool GuiRectButton::checkMousePosition(const sf::Vector2f& mousePosition) const
{
    return mousePosition.x > m_position.x - m_dimensions.x / 2 && mousePosition.x < m_position.x + m_dimensions.x / 2 &&
           mousePosition.y > m_position.y - m_dimensions.y / 2 && mousePosition.y < m_position.y + m_dimensions.y / 2;
}

bool GuiRectButton::onMouseHover(const sf::Vector2f& mousePosition)
{
    if (checkMousePosition(mousePosition) == true)
    {
        if (m_isClicked == false)
        {
            setState(GuiState::Hovered);
        }
        return true;
    }

    setState(GuiState::Normal);
    m_isClicked = false;
    return false;
}

bool GuiRectButton::onMouseClick(const sf::Vector2f& mousePosition)
{
    if (checkMousePosition(mousePosition) == true)
    {
        setState(GuiState::Clicked);
        m_isClicked = true;
        return true;
    }

    return false;
}

bool GuiRectButton::onMouseUnclick(const sf::Vector2f& mousePosition)
{
    if (checkMousePosition(mousePosition) == true && m_isClicked == true)
    {
        setState(GuiState::Hovered);
        m_isClicked = false;
        return true;
    }

    return false;
}

void GuiRectButton::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_shape.setPosition(m_position);
    m_text.setPosition(m_position);
}

void GuiRectButton::setDimensions(const sf::Vector2f& dimensions)
{
    m_dimensions = dimensions;
    m_shape.setSize(m_dimensions);
    m_shape.setOrigin(m_dimensions / 2.0f);
    m_text.setCharacterSize(m_dimensions.y + m_borderThickness * 2 - m_textPadding);
    m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2,
                     m_text.getFont()->getLineSpacing(m_text.getCharacterSize()) / 2);
}

void GuiRectButton::setStyle(GuiStyle style)
{
    m_style = style;
    switch (m_style)
    {
    default:
    case GuiStyle::White:
        m_bodyColor = sf::Color(255, 255, 255, 175);
        m_bodyColorHovered = sf::Color(200, 200, 200, 175);
        m_bodyColorClicked = sf::Color(150, 150, 175);
        m_borderColor = sf::Color(255, 255, 255, 200);
        m_borderColorHovered = sf::Color(255, 255, 255, 200);
        m_borderColorClicked = sf::Color(255, 255, 255, 200);
        m_textColor = sf::Color(5, 25, 100);
        m_textColorHovered = sf::Color(5, 25, 100);
        m_textColorClicked = sf::Color(5, 25, 100);
        break;
    case GuiStyle::Green:
        m_bodyColor = sf::Color(0, 255, 0, 100);
        m_bodyColorHovered = sf::Color(0, 200, 0, 100);
        m_bodyColorClicked = sf::Color(0, 150, 0);
        m_borderColor = sf::Color(0, 0, 0, 100);
        m_borderColorHovered = sf::Color(0, 0, 0, 100);
        m_borderColorClicked = sf::Color(0, 0, 0, 100);
        m_textColor = sf::Color(255, 255, 255);
        m_textColorHovered = sf::Color(255, 255, 255);
        m_textColorClicked = sf::Color(255, 255, 255);
        break;
    case GuiStyle::Red:
        m_bodyColor = sf::Color(255, 0, 0, 100);
        m_bodyColorHovered = sf::Color(200, 0, 0, 100);
        m_bodyColorClicked = sf::Color(150, 0, 0);
        m_borderColor = sf::Color(255, 255, 255, 100);
        m_borderColorHovered = sf::Color(255, 255, 255, 100);
        m_borderColorClicked = sf::Color(255, 255, 255, 100);
        m_textColor = sf::Color(255, 255, 255);
        m_textColorHovered = sf::Color(255, 255, 255);
        m_textColorClicked = sf::Color(255, 255, 255);
        break;
    case GuiStyle::Blue:
        m_bodyColor = sf::Color(0, 255, 255, 100);
        m_bodyColorHovered = sf::Color(0, 200, 200, 100);
        m_bodyColorClicked = sf::Color(0, 150, 150);
        m_borderColor = sf::Color(255, 255, 255, 100);
        m_borderColorHovered = sf::Color(255, 255, 255, 100);
        m_borderColorClicked = sf::Color(255, 255, 255, 100);
        m_textColor = sf::Color(255, 255, 255);
        m_textColorHovered = sf::Color(255, 255, 255);
        m_textColorClicked = sf::Color(255, 255, 255);
        break;
    }
}

void GuiRectButton::setState(GuiState state)
{
    if (m_state != state)
    {
        m_state = state;
        switch (m_state)
        {
        case GuiState::Normal:
            m_shape.setFillColor(m_bodyColor);
            m_shape.setOutlineColor(m_borderColor);
            m_text.setFillColor(m_textColor);
            break;
        case GuiState::Hovered:
            m_shape.setFillColor(m_bodyColorHovered);
            m_shape.setOutlineColor(m_borderColorHovered);
            m_text.setFillColor(m_textColorHovered);
            break;
        case GuiState::Clicked:
            m_shape.setFillColor(m_bodyColorClicked);
            m_shape.setOutlineColor(m_borderColorClicked);
            m_text.setFillColor(m_textColorClicked);
            break;
        }
    }
}

void GuiRectButton::setText(const std::string& text)
{
    m_text.setString(text);
    m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2,
                     m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
    m_text.setPosition(m_position);
}

// GuiRectSoundButton

GuiRectSoundButton::GuiRectSoundButton(const sf::Font& font, const sf::SoundBuffer& soundBuffer, const sf::Vector2f& position,
                                       const sf::Vector2f& dimensions, float borderThickness, int textPadding, const std::string& text,
                                       GuiStyle style)
    : GuiRectButton(font, position, dimensions, borderThickness, textPadding, text, style)
    , m_sound(soundBuffer)
{
}

void GuiRectSoundButton::setState(GuiState state)
{
    if (m_state != state)
    {
        GuiRectButton::setState(state);
        switch (m_state)
        {
        case GuiState::Hovered:
            m_sound.play();
            break;
        default:
            break;
        }
    }
}

// GuiSpriteButton

GuiSpriteButton::GuiSpriteButton(const sf::Texture& texture, const sf::Texture& textureHovered, const sf::Texture& textureClicked,
                                 const sf::Vector2f& position, const sf::Vector2f& dimensions)
    : m_texture(texture)
    , m_textureHovered(textureHovered)
    , m_textureClicked(textureClicked)
{
    setDimensions(dimensions);
    setPosition(position);
    m_shape.setTexture(&m_texture);
}

void GuiSpriteButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}

bool GuiSpriteButton::checkMousePosition(const sf::Vector2f& mousePosition) const // REPETITIVE
{
    return mousePosition.x > m_position.x - m_dimensions.x / 2 && mousePosition.x < m_position.x + m_dimensions.x / 2 &&
           mousePosition.y > m_position.y - m_dimensions.y / 2 && mousePosition.y < m_position.y + m_dimensions.y / 2;
}

bool GuiSpriteButton::onMouseHover(const sf::Vector2f& mousePosition) // REPETITIVE
{
    if (checkMousePosition(mousePosition) == true)
    {
        if (m_isClicked == false)
        {
            setState(GuiState::Hovered);
        }
        return true;
    }

    setState(GuiState::Normal);
    m_isClicked = false;
    return false;
}

bool GuiSpriteButton::onMouseClick(const sf::Vector2f& mousePosition) // REPETITIVE
{
    if (checkMousePosition(mousePosition) == true)
    {
        setState(GuiState::Clicked);
        m_isClicked = true;
        return true;
    }

    return false;
}

bool GuiSpriteButton::onMouseUnclick(const sf::Vector2f& mousePosition) // REPETITIVE
{
    if (checkMousePosition(mousePosition) == true && m_isClicked == true)
    {
        setState(GuiState::Hovered);
        m_isClicked = false;
        return true;
    }

    return false;
}

void GuiSpriteButton::setPosition(const sf::Vector2f& position) // REPETITIVE
{
    m_position = position;
    m_shape.setPosition(m_position);
}

void GuiSpriteButton::setDimensions(const sf::Vector2f& dimensions) // REPETITIVE
{
    m_dimensions = dimensions;
    m_shape.setSize(m_dimensions);
    m_shape.setOrigin(m_dimensions / 2.0f);
}

void GuiSpriteButton::setState(GuiState state)
{
    if (m_state != state)
    {
        m_state = state;
        switch (m_state)
        {
        case GuiState::Normal:
            m_shape.setTexture(&m_texture);
            break;
        case GuiState::Hovered:
            m_shape.setTexture(&m_textureHovered);
            break;
        case GuiState::Clicked:
            m_shape.setTexture(&m_textureClicked);
            break;
        }
    }
}

// GuiScrollbar

GuiScrollbar::GuiScrollbar(const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, float sliderWidth,
                           float borderThickness, double value)
    : m_borderThickness(borderThickness)
    , m_value(value)
{
    setStyle(style);
    setDimensions(dimensions, sliderWidth);
    setPosition(position);
    m_shape.setOutlineThickness(m_borderThickness);
    m_shape.setFillColor(m_bodyColor);
    m_shape.setOutlineColor(m_borderColor);
    m_slider.setFillColor(m_sliderColor);
}

void GuiScrollbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_slider, states);
}

bool GuiScrollbar::checkMousePosition(const sf::Vector2f& mousePosition) const
{
    return mousePosition.x >= m_position.x - m_dimensions.x / 2 - m_borderThickness &&
           mousePosition.x <= m_position.x + m_dimensions.x / 2 + m_borderThickness &&
           mousePosition.y >= m_position.y - m_dimensions.y / 2 - m_borderThickness &&
           mousePosition.y <= m_position.y + m_dimensions.y / 2 + m_borderThickness;
}

bool GuiScrollbar::checkMousePositionOnSlider(const sf::Vector2f& mousePosition) const
{
    return mousePosition.x >= m_sliderPosition.x - m_sliderDimensions.x / 2 &&
           mousePosition.x <= m_sliderPosition.x + m_sliderDimensions.x / 2 &&
           mousePosition.y >= m_sliderPosition.y - m_sliderDimensions.y / 2 &&
           mousePosition.y <= m_sliderPosition.y + m_sliderDimensions.y / 2;
}

void GuiScrollbar::moveSlider(const sf::Vector2f& position)
{
    if (m_isVertical)
    {
        // Check if slider can move vertically
        if (m_sliderDimensions.y < m_dimensions.y + m_borderThickness * 2)
        {
            m_sliderPosition.y = position.y;

            checkSliderCollision();

            // m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2 : width the slider can travel
            m_value = (m_sliderPosition.y - m_sliderDimensions.y / 2 - m_position.y + m_dimensions.y / 2 + m_borderThickness) /
                      (m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2);
        }
    }
    else
    {
        // Check if slider can move horizontally
        if (m_sliderDimensions.x < m_dimensions.x + m_borderThickness * 2)
        {
            m_sliderPosition.x = position.x;

            checkSliderCollision();

            // m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2 : width the slider can travel
            m_value = (m_sliderPosition.x - m_sliderDimensions.x / 2 - m_position.x + m_dimensions.x / 2 + m_borderThickness) /
                      (m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2);
        }
    }
}

void GuiScrollbar::checkSliderCollision()
{
    if (m_isVertical)
    {
        // Edge collision
        if (m_sliderPosition.y - m_sliderDimensions.y / 2 < m_position.y - m_dimensions.y / 2 - m_borderThickness)
        {
            m_sliderPosition.y = m_position.y - m_dimensions.y / 2 - m_borderThickness + m_sliderDimensions.y / 2;
        }
        else if (m_sliderPosition.y + m_sliderDimensions.y / 2 > m_position.y + m_dimensions.y / 2 + m_borderThickness)
        {
            m_sliderPosition.y = m_position.y + m_dimensions.y / 2 + m_borderThickness - m_sliderDimensions.y / 2;
        }
    }
    else
    {
        // Edge collision
        if (m_sliderPosition.x - m_sliderDimensions.x / 2 < m_position.x - m_dimensions.x / 2 - m_borderThickness)
        {
            m_sliderPosition.x = m_position.x - m_dimensions.x / 2 - m_borderThickness + m_sliderDimensions.x / 2;
        }
        else if (m_sliderPosition.x + m_sliderDimensions.x / 2 > m_position.x + m_dimensions.x / 2 + m_borderThickness)
        {
            m_sliderPosition.x = m_position.x + m_dimensions.x / 2 + m_borderThickness - m_sliderDimensions.x / 2;
        }
    }

    m_slider.setPosition(m_sliderPosition);
}

bool GuiScrollbar::onMouseHover(const sf::Vector2f& mousePosition)
{
    if (m_isClicked == true)
    {
        moveSlider(mousePosition);
        return true;
    }

    // If m_isClicked is false
    if (checkMousePositionOnSlider(mousePosition) == true)
    {
        setState(GuiState::Hovered);
        return true;
    }

    setState(GuiState::Normal);
    m_isClicked = false;
    return false;
}

bool GuiScrollbar::onMouseClick(const sf::Vector2f& mousePosition)
{
    if (checkMousePosition(mousePosition) == true)
    {
        setState(GuiState::Clicked);
        m_isClicked = true;
        moveSlider(mousePosition);
        return true;
    }

    return false;
}

bool GuiScrollbar::onMouseUnclick(const sf::Vector2f& mousePosition)
{
    if (m_isClicked == true)
    {
        if (checkMousePosition(mousePosition) == true)
        {
            setState(GuiState::Hovered);
        }
        else
        {
            setState(GuiState::Normal);
        }
        m_isClicked = false;
        return true;
    }

    return false;
}

bool GuiScrollbar::onMouseScroll(float scrollAmount)
{
    if (scrollAmount != 0)
    {
        if (m_isVertical)
        {
            // Move slider relative to current position, taller sliders move more than shorter ones
            moveSlider(
                sf::Vector2f(0, m_sliderPosition.y - 40 * scrollAmount * m_sliderDimensions.y / (m_dimensions.y - m_borderThickness * 2)));
        }
        else
        {
            // Move slider relative to current position, wider sliders move more than thinner ones
            moveSlider(
                sf::Vector2f(m_sliderPosition.x - 40 * scrollAmount * m_sliderDimensions.x / (m_dimensions.x - m_borderThickness * 2), 0));
        }

        return true;
    }

    return false;
}

void GuiScrollbar::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_shape.setPosition(m_position);

    if (m_isVertical == true)
    {
        m_sliderPosition =
            sf::Vector2f(m_position.x, m_position.y + (m_value - 0.5) * (m_dimensions.y - m_sliderDimensions.y) - m_borderThickness);
    }
    else
    {
        m_sliderPosition =
            sf::Vector2f(m_position.x + (m_value - 0.5) * (m_dimensions.x - m_sliderDimensions.x) - m_borderThickness, m_position.y);
    }

    m_slider.setPosition(m_sliderPosition);
}

void GuiScrollbar::setDimensions(const sf::Vector2f& dimensions, float sliderWidth)
{
    m_dimensions = dimensions;
    m_shape.setSize(m_dimensions);
    m_shape.setOrigin(m_dimensions / 2.0f);

    if (m_dimensions.y > m_dimensions.x)
    {
        m_isVertical = true;
    }
    else
    {
        m_isVertical = false;
    }

    setSliderWidth(sliderWidth);
}

void GuiScrollbar::setSliderWidth(float sliderWidth)
{
    if (m_isVertical == true)
    {
        if (sliderWidth > m_dimensions.y + m_borderThickness * 2)
        {
            sliderWidth = m_dimensions.y + m_borderThickness * 2;
        }
        m_sliderDimensions = sf::Vector2f(m_dimensions.x + m_borderThickness * 2, sliderWidth);
    }
    else
    {
        if (sliderWidth > m_dimensions.x + m_borderThickness * 2)
        {
            sliderWidth = m_dimensions.x + m_borderThickness * 2;
        }
        m_sliderDimensions = sf::Vector2f(sliderWidth, m_dimensions.y + m_borderThickness * 2);
    }
    m_slider.setSize(m_sliderDimensions);
    m_slider.setOrigin(m_sliderDimensions / 2.0f);
}

void GuiScrollbar::setStyle(GuiStyle style)
{
    m_style = style;
    switch (m_style)
    {
    default:
    case GuiStyle::White:
        m_bodyColor = sf::Color(255, 255, 255, 175);
        m_sliderColor = sf::Color(150, 150, 150);
        m_sliderColorHovered = sf::Color(100, 100, 100);
        m_borderColor = sf::Color(255, 255, 255, 175);
        break;
    case GuiStyle::Green:
        m_bodyColor = sf::Color(0, 255, 0, 100);
        m_sliderColor = sf::Color(0, 200, 0);
        m_sliderColorHovered = sf::Color(0, 150, 0);
        m_borderColor = sf::Color(0, 0, 0, 100);
        break;
    case GuiStyle::Red:
        m_bodyColor = sf::Color(255, 0, 0, 100);
        m_sliderColor = sf::Color(200, 0, 0);
        m_sliderColorHovered = sf::Color(150, 0, 0);
        m_borderColor = sf::Color(255, 255, 255, 100);
        break;
    case GuiStyle::Blue:
        m_bodyColor = sf::Color(0, 255, 255, 100);
        m_sliderColor = sf::Color(0, 200, 200);
        m_sliderColorHovered = sf::Color(0, 150, 150);
        m_borderColor = sf::Color(255, 255, 255, 100);
        break;
    }
}

void GuiScrollbar::setState(GuiState state)
{
    if (m_state != state)
    {
        m_state = state;
        switch (m_state)
        {
        case GuiState::Normal:
            m_slider.setFillColor(m_sliderColor);
            break;
        case GuiState::Hovered:
            m_slider.setFillColor(m_sliderColorHovered);
            break;
        case GuiState::Clicked:
            m_slider.setFillColor(m_sliderColorHovered);
            break;
        }
    }
}

void GuiScrollbar::setValue(double value)
{
    if (value > 1.0)
    {
        value = 1.0;
    }
    m_value = value;

    if (m_isVertical)
    {
        // m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2 : width the slider can travel
        m_sliderPosition.y = m_position.y - m_dimensions.y / 2 - m_borderThickness + m_sliderDimensions.y / 2 +
                             (m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2) * m_value;
    }
    else
    {
        // m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2 : width the slider can travel
        m_sliderPosition.x = m_position.x - m_dimensions.x / 2 - m_borderThickness + m_sliderDimensions.x / 2 +
                             (m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2) * m_value;
    }

    checkSliderCollision();
}

// GuiSlider

GuiSlider::GuiSlider(const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, float sliderWidth, float borderThickness,
                     double maxValue, double value)
    : GuiScrollbar(position, dimensions, style, sliderWidth, borderThickness)
    , m_maxValue(maxValue)
{
    setValue(value);
}

void GuiSlider::moveSlider(const sf::Vector2f& position)
{
    if (m_isVertical == true)
    {
        // Position is equal to : (start) + (length of slider traversed by increase of 1 in m_value) * round((position -
        // start) / (length of slider traversed by increase of 1 in m_value))
        m_sliderPosition.y =
            m_position.y - m_dimensions.y / 2 - m_borderThickness + m_sliderDimensions.y / 2 +
            ((m_dimensions.y + 2 * m_borderThickness - m_sliderDimensions.y) / m_maxValue) *
                std::round((position.y - (m_position.y - m_dimensions.y / 2 - m_borderThickness + m_sliderDimensions.y / 2)) /
                           ((m_dimensions.y + 2 * m_borderThickness - m_sliderDimensions.y) / m_maxValue));

        checkSliderCollision();

        // m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2 : width the slider can travel
        m_value = (m_sliderPosition.y - m_sliderDimensions.y / 2 - m_position.y + m_dimensions.y / 2 + m_borderThickness) /
                  (m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2) * m_maxValue;
    }
    else
    {
        // Position is equal to : (start) + (length of slider traversed by increase of 1 in m_value) * round((position -
        // start) / (length of slider traversed by increase of 1 in m_value))
        m_sliderPosition.x =
            m_position.x - m_dimensions.x / 2 - m_borderThickness + m_sliderDimensions.x / 2 +
            ((m_dimensions.x + 2 * m_borderThickness - m_sliderDimensions.x) / m_maxValue) *
                std::round((position.x - (m_position.x - m_dimensions.x / 2 - m_borderThickness + m_sliderDimensions.x / 2)) /
                           ((m_dimensions.x + 2 * m_borderThickness - m_sliderDimensions.x) / m_maxValue));

        checkSliderCollision();

        // m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2 : width the slider can travel
        m_value = (m_sliderPosition.x - m_sliderDimensions.x / 2 - m_position.x + m_dimensions.x / 2 + m_borderThickness) /
                  (m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2) * m_maxValue;
    }
}

void GuiSlider::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_shape.setPosition(position);

    if (m_isVertical == true)
    {
        m_sliderPosition = sf::Vector2f(m_position.x,
                                        m_position.y - m_dimensions.y / 2 - m_borderThickness + m_sliderDimensions.y / 2 +
                                            (m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2) * m_value / m_maxValue);
    }
    else
    {
        m_sliderPosition = sf::Vector2f(m_position.x - m_dimensions.x / 2 - m_borderThickness + m_sliderDimensions.x / 2 +
                                            (m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2) * m_value / m_maxValue,
                                        m_position.y);
    }

    m_slider.setPosition(m_sliderPosition);
}

void GuiSlider::setValue(double value)
{
    if (value > m_maxValue)
    {
        value = m_maxValue;
    }
    m_value = value;

    if (m_isVertical)
    {
        // m_dimensions.y - m_sliderDimensions.y + m_borderThickness * 2 : width the slider can travel
        m_sliderPosition.y = m_position.y - m_dimensions.y / 2 - m_borderThickness + m_sliderDimensions.y / 2 +
                             ((m_dimensions.y + 2 * m_borderThickness - m_sliderDimensions.y) / m_maxValue) * m_value;
    }
    else
    {
        // m_dimensions.x - m_sliderDimensions.x + m_borderThickness * 2 : width the slider can travel
        m_sliderPosition.x = m_position.x - m_dimensions.x / 2 - m_borderThickness + m_sliderDimensions.x / 2 +
                             ((m_dimensions.x + 2 * m_borderThickness - m_sliderDimensions.x) / m_maxValue) * m_value;
    }

    checkSliderCollision();
    m_slider.setPosition(m_sliderPosition);
}

// GuiTextSlider

GuiTextSlider::GuiTextSlider(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style,
                             const std::string& text, float sliderWidth, float borderThickness, int textPadding, double maxValue,
                             double value)
    : GuiSlider(position, dimensions, style, sliderWidth, borderThickness, maxValue, value)
    , m_textPadding(textPadding)
{
    setFont(font);
    setText(text);
    m_text.setFillColor(m_textColor);
}

void GuiTextSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GuiScrollbar::draw(target, states);
    target.draw(m_text, states);
}

void GuiTextSlider::moveSlider(const sf::Vector2f& position)
{
    GuiSlider::moveSlider(position);
    setText(m_baseString);
}

void GuiTextSlider::setPosition(const sf::Vector2f& position)
{
    GuiSlider::setPosition(position);
    m_text.setPosition(m_position);
}

void GuiTextSlider::setDimensions(const sf::Vector2f& dimensions, float sliderWidth)
{
    GuiScrollbar::setDimensions(dimensions, sliderWidth);

    if (m_isVertical)
    {
        m_text.setCharacterSize(m_dimensions.x + (m_borderThickness * 2) - m_textPadding);
        m_text.setRotation(90);
    }
    else
    {
        m_text.setCharacterSize(m_dimensions.y + (m_borderThickness * 2) - m_textPadding);
    }

    m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2,
                     m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
}

void GuiTextSlider::setStyle(GuiStyle style)
{
    GuiScrollbar::setStyle(style);

    switch (m_style)
    {
    default:
    case GuiStyle::White:
        m_textColor = sf::Color(255, 255, 255);
        break;
    case GuiStyle::Green:
        m_textColor = sf::Color(255, 255, 255);
        break;
    case GuiStyle::Red:
        m_textColor = sf::Color(255, 255, 255);
        break;
    case GuiStyle::Blue:
        m_textColor = sf::Color(255, 255, 255);
        break;
    }
}

void GuiTextSlider::setValue(double value)
{
    GuiSlider::setValue(value);
    setText(m_baseString);
}

void GuiTextSlider::setText(const std::string& text)
{
    m_baseString = text;

    m_text.setString(m_baseString + '(' + std::to_string(static_cast<int>(m_value)) + ')');

    m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2,
                     m_text.getLocalBounds().top + m_text.getLocalBounds().height / 2);
    m_text.setPosition(m_position);
}

// ProgressBar

ProgressBar::ProgressBar(const sf::Vector2f& position, const sf::Vector2f& boxDimensions, sf::Color backgroundColor, sf::Color borderColor,
                         sf::Color progressBarColor, int borderThickness, double fraction)
{
    setDimensions(boxDimensions);
    setBackgroundColor(backgroundColor);
    setBorderColor(borderColor);
    setProgressBarColor(progressBarColor);
    setBorderThickness(borderThickness);
    setFraction(fraction);
    setPosition(position);
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_box, states);
    target.draw(m_progressBar, states);
}

void ProgressBar::calculateFraction()
{
    m_fraction = static_cast<double>(m_progress) / m_total;
}

void ProgressBar::calculateProgress()
{
    m_progress = static_cast<unsigned int>(m_fraction * m_total);
}

void ProgressBar::updateProgressBarPosition()
{
    m_progressBar.setPosition(m_box.getPosition().x - m_box.getSize().x / 2 + m_progressBar.getSize().x / 2 - m_box.getOutlineThickness(),
                              m_box.getPosition().y);
}

void ProgressBar::setPosition(const sf::Vector2f& position)
{
    m_box.setPosition(position);
    updateProgressBarPosition();
}

void ProgressBar::setDimensions(const sf::Vector2f& dimensions)
{
    m_box.setSize(dimensions);
    m_box.setOrigin(dimensions / 2.0f);
    updateProgressBarPosition();
    setFraction(m_fraction);
}

void ProgressBar::setBorderThickness(float borderThickness)
{
    m_box.setOutlineThickness(borderThickness);
    updateProgressBarPosition();
    setFraction(m_fraction);
}

void ProgressBar::setProgress(unsigned int progress)
{
    m_progress = progress;
    calculateFraction();
    setFraction(m_fraction);
}

void ProgressBar::setTotal(unsigned int total)
{
    m_total = total;
    calculateFraction();
    setFraction(m_fraction);
}

void ProgressBar::setFraction(double fraction)
{
    m_fraction = fraction;
    calculateProgress();
    m_progressBar.setSize(sf::Vector2f((m_box.getSize().x + 2 * m_box.getOutlineThickness()) * m_fraction,
                                       m_box.getSize().y + 2 * m_box.getOutlineThickness()));
    m_progressBar.setOrigin(m_progressBar.getSize() / 2.0f);
    updateProgressBarPosition();
}

#ifndef GUI_H
#define GUI_H

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// DEPRECATED

enum class GuiStyle
{
    White,
    Green,
    Red,
    Blue,
};

enum class GuiState
{
    Normal,
    Hovered,
    Clicked
};

// Base interactive GUI class
class InteractiveGui : public sf::Drawable
{
protected:
    sf::Vector2f m_position;

    GuiState m_state;

    bool m_isClicked;

    // Constructor
    InteractiveGui();

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    virtual bool checkMousePosition(const sf::Vector2f& mousePosition) const = 0;

public:
    // Destructor
    virtual ~InteractiveGui() {}

    // Functions
    virtual bool onMouseHover(const sf::Vector2f& mousePosition) = 0;
    virtual bool onMouseClick(const sf::Vector2f& mousePosition) = 0;
    virtual bool onMouseUnclick(const sf::Vector2f& mousePosition) = 0;

    // Setters
    virtual void setPosition(const sf::Vector2f& position) { m_position = position; }
    virtual void setState(GuiState state) { m_state = state; }

    // Getters
    const sf::Vector2f& getPosition() const { return m_position; }
    virtual GuiState getState() const { return m_state; }
};

// GuiRectButton
class GuiRectButton : public InteractiveGui
{
protected:
    sf::Vector2f m_dimensions;
    float m_borderThickness;
    float m_textPadding;

    sf::RectangleShape m_shape;
    sf::Text m_text;

    GuiStyle m_style;

    sf::Color m_bodyColor;
    sf::Color m_bodyColorHovered;
    sf::Color m_bodyColorClicked;
    sf::Color m_borderColor;
    sf::Color m_borderColorHovered;
    sf::Color m_borderColorClicked;
    sf::Color m_textColor;
    sf::Color m_textColorHovered;
    sf::Color m_textColorClicked;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool checkMousePosition(const sf::Vector2f& mousePosition) const override;

public:
    // Constructor
    GuiRectButton(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions, float borderThickness,
                  float textPadding, const std::string& text, GuiStyle style);

    // Functions
    virtual bool onMouseHover(const sf::Vector2f& mousePosition) override;
    virtual bool onMouseClick(const sf::Vector2f& mousePosition) override;
    virtual bool onMouseUnclick(const sf::Vector2f& mousePosition) override;

    // Setters
    virtual void setPosition(const sf::Vector2f& position) override;
    virtual void setDimensions(const sf::Vector2f& dimensions);
    virtual void setStyle(GuiStyle style);
    virtual void setState(GuiState state) override;
    virtual void setFont(const sf::Font& font) { m_text.setFont(font); }
    void setText(const std::string& text);

    void setBorderThickness(float borderThickness) { m_borderThickness = borderThickness; }
    void setTextPadding(float textPadding) { m_textPadding = textPadding; }

    void setBodyColor(sf::Color color) { m_bodyColor = color; }
    void setBodyColorHovered(sf::Color color) { m_bodyColorHovered = color; }
    void setBodyColorClicked(sf::Color color) { m_bodyColorClicked = color; }
    void setBorderColor(sf::Color color) { m_borderColor = color; }
    void setBorderColorHovered(sf::Color color) { m_borderColorHovered = color; }
    void setBorderColorClicked(sf::Color color) { m_borderColorClicked = color; }
    void setTextColor(sf::Color color) { m_textColor = color; }
    void setTextColorHovered(sf::Color color) { m_textColorHovered = color; }
    void setTextColorClicked(sf::Color color) { m_textColorClicked = color; }

    // Getters
    const sf::Vector2f& getDimensions() const { return m_dimensions; }
    std::string getText() const { return m_text.getString(); }
};

// GuiRectSoundButton
class GuiRectSoundButton : public GuiRectButton
{
private:
    sf::Sound m_sound;

public:
    // Constructor
    GuiRectSoundButton(const sf::Font& font, const sf::SoundBuffer& soundBuffer, const sf::Vector2f& position,
                       const sf::Vector2f& dimensions, float borderThickness, int textPadding, const std::string& text, GuiStyle style);

    // Setters
    void setState(GuiState state) override;
    void setVolume(float volume) { m_sound.setVolume(volume); }
};

// GuiSpriteButton
class GuiSpriteButton : public InteractiveGui
{
private:
    sf::Vector2f m_dimensions;

    sf::RectangleShape m_shape;
    sf::Texture m_texture;
    sf::Texture m_textureHovered;
    sf::Texture m_textureClicked;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool checkMousePosition(const sf::Vector2f& mousePosition) const override;

public:
    // Constructor
    GuiSpriteButton(const sf::Texture& texture, const sf::Texture& textureHovered, const sf::Texture& textureClicked,
                    const sf::Vector2f& position, const sf::Vector2f& dimensions);

    // Functions
    bool onMouseHover(const sf::Vector2f& mousePosition) override;
    bool onMouseClick(const sf::Vector2f& mousePosition) override;
    bool onMouseUnclick(const sf::Vector2f& mousePosition) override;

    // Setters
    void setPosition(const sf::Vector2f& position) override;
    void setDimensions(const sf::Vector2f& dimensions);
    void setState(GuiState state) override;

    void setTexture(const sf::Texture& texture) { m_texture = texture; }
    void setTextureHovered(const sf::Texture& texture) { m_textureHovered = texture; }
    void setTextureClicked(const sf::Texture& texture) { m_textureClicked = texture; }

    // Getters
    const sf::Vector2f& getDimensions() const { return m_dimensions; }
};

// GuiScrollbar
class GuiScrollbar : public InteractiveGui
{
protected:
    sf::Vector2f m_dimensions;
    sf::Vector2f m_sliderPosition;
    sf::Vector2f m_sliderDimensions;
    float m_borderThickness;
    bool m_isVertical;

    sf::RectangleShape m_shape;
    sf::RectangleShape m_slider;

    GuiStyle m_style;

    sf::Color m_bodyColor;
    sf::Color m_sliderColor;
    sf::Color m_sliderColorHovered;
    sf::Color m_borderColor;

    double m_value;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool checkMousePosition(const sf::Vector2f& mousePosition) const override;
    bool checkMousePositionOnSlider(const sf::Vector2f& mousePosition) const;
    virtual void moveSlider(const sf::Vector2f& position);
    void checkSliderCollision();

public:
    // Constructor
    GuiScrollbar(const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, float sliderWidth, float borderThickness = 0,
                 double value = 0);

    // Functions
    bool onMouseHover(const sf::Vector2f& mousePosition) override;
    bool onMouseClick(const sf::Vector2f& mousePosition) override;
    bool onMouseUnclick(const sf::Vector2f& mousePosition) override;
    bool onMouseScroll(float scrollAmount);

    // Setters
    virtual void setPosition(const sf::Vector2f& position) override;
    virtual void setDimensions(const sf::Vector2f& dimensions, float sliderWidth);
    void setSliderWidth(float sliderWidth);
    void setState(GuiState state) override;
    virtual void setStyle(GuiStyle style);

    void setBorderThickness(float borderThickness) { m_borderThickness = borderThickness; }

    void setBodyColor(sf::Color color) { m_bodyColor = color; }
    void setSliderColor(sf::Color color) { m_sliderColor = color; }
    void setSliderColorHovered(sf::Color color) { m_sliderColorHovered = color; }
    void setBorderColor(sf::Color color) { m_borderColor = color; }

    virtual void setValue(double value);

    // Getters
    double getValue() const { return m_value; }
    const sf::Vector2f& getSliderPosition() const { return m_sliderPosition; }
    const sf::Vector2f& getDimensions() const { return m_dimensions; }
    const sf::Vector2f& getSliderDimensions() const { return m_sliderDimensions; }
};

// GuiSlider
class GuiSlider : public GuiScrollbar
{
protected:
    double m_maxValue;

    // Functions
    virtual void moveSlider(const sf::Vector2f& position) override;

public:
    // Constructor
    GuiSlider(const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, float sliderWidth, float borderThickness = -3,
              double maxValue = 100, double value = 0);

    bool onMouseScroll(int scrollAmount) = delete;

    // Setters
    virtual void setPosition(const sf::Vector2f& position) override;
    virtual void setValue(double value) override;
};

// GuiTextSlider
class GuiTextSlider : public GuiSlider
{
protected:
    std::string m_baseString;
    sf::Text m_text;
    float m_textPadding;
    sf::Color m_textColor;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void moveSlider(const sf::Vector2f& position) override;

public:
    // Constructor
    GuiTextSlider(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style,
                  const std::string& text, float sliderWidth, float borderThickness = -3, int textPadding = 0, double maxValue = 100,
                  double value = 0);

    // Setters
    virtual void setPosition(const sf::Vector2f& position) override;
    virtual void setDimensions(const sf::Vector2f& dimensions, float sliderWidth) override;
    virtual void setStyle(GuiStyle style) override;
    virtual void setValue(double value) override;
    void setFont(const sf::Font& font) { m_text.setFont(font); }
    void setText(const std::string& text);
    void setTextPadding(float textPadding) { m_textPadding = textPadding; }
    void setTextColor(sf::Color color) { m_textColor = color; }
};

// ProgressBar
class ProgressBar : public sf::Drawable
{
protected:
    sf::RectangleShape m_progressBar;
    sf::RectangleShape m_box;

    unsigned int m_progress;
    unsigned int m_total;
    double m_fraction;

    // Functions
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void calculateFraction();
    void calculateProgress();
    void updateProgressBarPosition();

public:
    // Constructor
    ProgressBar(const sf::Vector2f& position, const sf::Vector2f& boxDimensions, sf::Color backgroundColor, sf::Color borderColor,
                sf::Color progressBarColor, int borderThickness, double fraction);

    // Setters
    void setPosition(const sf::Vector2f& position);
    void setDimensions(const sf::Vector2f& dimensions);
    void setBorderThickness(float borderThickness);
    void setBackgroundColor(sf::Color backgroundColor) { m_box.setFillColor(backgroundColor); }
    void setBorderColor(sf::Color borderColor) { m_box.setOutlineColor(borderColor); }
    void setProgressBarColor(sf::Color progressBarColor) { m_progressBar.setFillColor(progressBarColor); }
    void setProgress(unsigned int progress);
    void setTotal(unsigned int total);
    void setFraction(double fraction);

    // Getters
    const sf::Vector2f& getPosition() const { return m_box.getPosition(); }
    const sf::Vector2f& getDimensions() const { return m_box.getSize(); }
    unsigned int getProgress() const { return m_progress; }
    unsigned int getTotal() const { return m_total; }
    double getFraction() const { return m_fraction; }
};

#endif // GUI_H

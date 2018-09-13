#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

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
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override = 0;
    virtual bool CheckMousePosition(const sf::Vector2f& mousePosition) const = 0;

public:
    // Destructor
    virtual ~InteractiveGui() {}

    // Functions
    virtual bool OnMouseHover(const sf::Vector2f& mousePosition) = 0;
    virtual bool OnMouseClick(const sf::Vector2f& mousePosition) = 0;
    virtual bool OnMouseUnclick(const sf::Vector2f& mousePosition) = 0;

    // Setters
    virtual void SetPosition(const sf::Vector2f& position) {m_position = position;}
    virtual void SetState(GuiState state) {m_state = state;}

    // Getters
    const sf::Vector2f& GetPosition() const {return m_position;}
    virtual GuiState GetState() const {return m_state;}
};

// GuiRectButton
class GuiRectButton : public InteractiveGui
{
protected:
    sf::Vector2f m_dimensions;
    float m_borderThickness;
    int m_textPadding;

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
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    bool CheckMousePosition(const sf::Vector2f& mousePosition) const override;

public:
    // Constructor
    GuiRectButton(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions,
                  float borderThickness, int textPadding, const std::string& text, GuiStyle style);

    // Functions
    virtual bool OnMouseHover(const sf::Vector2f& mousePosition) override;
    virtual bool OnMouseClick(const sf::Vector2f& mousePosition) override;
    virtual bool OnMouseUnclick(const sf::Vector2f& mousePosition) override;

    // Setters
    virtual void SetPosition(const sf::Vector2f& position) override;
    virtual void SetDimensions(const sf::Vector2f& dimensions);
    virtual void SetStyle(GuiStyle style);
    virtual void SetState(GuiState state) override;
    virtual void SetFont(const sf::Font& font) {m_text.setFont(font);}
    void SetText(const std::string& text);

    void SetBorderThickness(float borderThickness) {m_borderThickness = borderThickness;}
    void SetTextPadding(float textPadding) {m_textPadding = textPadding;}

    void SetBodyColor(sf::Color color) {m_bodyColor = color;}
    void SetBodyColorHovered(sf::Color color) {m_bodyColorHovered = color;}
    void SetBodyColorClicked(sf::Color color) {m_bodyColorClicked = color;}
    void SetBorderColor(sf::Color color) {m_borderColor = color;}
    void SetBorderColorHovered(sf::Color color) {m_borderColorHovered = color;}
    void SetBorderColorClicked(sf::Color color) {m_borderColorClicked = color;}
    void SetTextColor(sf::Color color) {m_textColor = color;}
    void SetTextColorHovered(sf::Color color) {m_textColorHovered = color;}
    void SetTextColorClicked(sf::Color color) {m_textColorClicked = color;}

    // Getters
    const sf::Vector2f& GetDimensions() const {return m_dimensions;}
    std::string GetText() const {return m_text.getString();}
};

// GuiRectSoundButton
class GuiRectSoundButton : public GuiRectButton
{
private:
    sf::Sound m_sound;

public:
    // Constructor
    GuiRectSoundButton(const sf::Font& font, const sf::SoundBuffer& soundBuffer, const sf::Vector2f& position, const sf::Vector2f& dimensions,
                       float borderThickness, int textPadding, const std::string& text, GuiStyle style);

    // Setters
    void SetState(GuiState state) override;
    void SetVolume(float volume) {m_sound.setVolume(volume);}
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
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    bool CheckMousePosition(const sf::Vector2f& mousePosition) const override;

public:
    // Constructor
    GuiSpriteButton(const sf::Texture& texture, const sf::Texture& textureHovered, const sf::Texture& textureClicked,
                    const sf::Vector2f& position, const sf::Vector2f& dimensions);

    // Functions
    bool OnMouseHover(const sf::Vector2f& mousePosition) override;
    bool OnMouseClick(const sf::Vector2f& mousePosition) override;
    bool OnMouseUnclick(const sf::Vector2f& mousePosition) override;

    // Setters
    void SetPosition(const sf::Vector2f& position) override;
    void SetDimensions(const sf::Vector2f& dimensions);
    void SetState(GuiState state) override;

    void SetTexture(const sf::Texture& texture) {m_texture = texture;}
    void SetTextureHovered(const sf::Texture& texture) {m_textureHovered = texture;}
    void SetTextureClicked(const sf::Texture& texture) {m_textureClicked = texture;}

    // Getters
    const sf::Vector2f& GetDimensions() const {return m_dimensions;}
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
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    bool CheckMousePosition(const sf::Vector2f& mousePosition) const override;
    bool CheckMousePositionOnSlider(const sf::Vector2f& mousePosition) const;
    virtual void MoveSlider(const sf::Vector2f& position);
    void CheckSliderCollision();

public:
    // Constructor
    GuiScrollbar(const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, float sliderWidth, float borderThickness = 0, double value = 0);

    // Functions
    bool OnMouseHover(const sf::Vector2f& mousePosition) override;
    bool OnMouseClick(const sf::Vector2f& mousePosition) override;
    bool OnMouseUnclick(const sf::Vector2f& mousePosition) override;
    bool OnMouseScroll(float scrollAmount);

    // Setters
    virtual void SetPosition(const sf::Vector2f& position) override;
    virtual void SetDimensions(const sf::Vector2f& dimensions, float sliderWidth);
    void SetSliderWidth(float sliderWidth);
    void SetState(GuiState state) override;
    virtual void SetStyle(GuiStyle style);

    void SetBorderThickness(float borderThickness) {m_borderThickness = borderThickness;}

    void SetBodyColor(sf::Color color) {m_bodyColor = color;}
    void SetSliderColor(sf::Color color) {m_sliderColor = color;}
    void SetSliderColorHovered(sf::Color color) {m_sliderColorHovered = color;}
    void SetBorderColor(sf::Color color) {m_borderColor = color;}

    virtual void SetValue(double value);

    // Getters
    double GetValue() const {return m_value;}
    const sf::Vector2f& GetSliderPosition() const {return m_sliderPosition;}
    const sf::Vector2f& GetDimensions() const {return m_dimensions;}
    const sf::Vector2f& GetSliderDimensions() const {return m_sliderDimensions;}
};

// GuiSlider
class GuiSlider : public GuiScrollbar
{
protected:
    double m_maxValue;

    // Functions
    virtual void MoveSlider(const sf::Vector2f& position) override;

public:
    // Constructor
    GuiSlider(const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, float sliderWidth, float borderThickness = -3, double maxValue = 100, double value = 0);

    bool OnMouseScroll(int scrollAmount) = delete;

    // Setters
    virtual void SetPosition(const sf::Vector2f& position) override;
    virtual void SetValue(double value) override;

    // Getters
    double GetValue() const {return m_value;}
};

// GuiTextSlider
class GuiTextSlider : public GuiSlider
{
protected:
    std::string m_baseString;
    sf::Text m_text;
    int m_textPadding;
    sf::Color m_textColor;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    virtual void MoveSlider(const sf::Vector2f& position) override;

public:
    // Constructor
    GuiTextSlider(const sf::Font& font, const sf::Vector2f& position, const sf::Vector2f& dimensions, GuiStyle style, const std::string& text,
                  float sliderWidth, float borderThickness = -3, int textPadding = 0, double maxValue = 100, double value = 0);

    // Setters
    virtual void SetPosition(const sf::Vector2f& position) override;
    virtual void SetDimensions(const sf::Vector2f& dimensions, float sliderWidth) override;
    virtual void SetStyle(GuiStyle style) override;
    virtual void SetValue(double value) override;
    void SetFont(const sf::Font& font) {m_text.setFont(font);}
    void SetText(const std::string& text);
    void SetTextPadding(float textPadding) {m_textPadding = textPadding;}
    void SetTextColor(sf::Color color) {m_textColor = color;}
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
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;
    void CalculateFraction();
    void CalculateProgress();
    void UpdateProgressBarPosition();

public:
    // Constructor
    ProgressBar(const sf::Vector2f& position, const sf::Vector2f& boxDimensions, sf::Color backgroundColor,
                sf::Color borderColor, sf::Color progressBarColor, int borderThickness, double fraction);

    // Setters
    void SetPosition(const sf::Vector2f& position);
    void SetDimensions(const sf::Vector2f& dimensions);
    void SetBorderThickness(float borderThickness);
    void SetBackgroundColor(sf::Color backgroundColor) {m_box.setFillColor(backgroundColor);}
    void SetBorderColor(sf::Color borderColor) {m_box.setOutlineColor(borderColor);}
    void SetProgressBarColor(sf::Color progressBarColor) {m_progressBar.setFillColor(progressBarColor);}
    void SetProgress(unsigned int progress);
    void SetTotal(unsigned int total);
    void SetFraction(double fraction);

    // Getters
    const sf::Vector2f& GetPosition() const {return m_box.getPosition();}
    const sf::Vector2f& GetDimensions() const {return m_box.getSize();}
    unsigned int GetProgress() const {return m_progress;}
    unsigned int GetTotal() const {return m_total;}
    double GetFraction() const {return m_fraction;}
};

#endif // GUI_H_INCLUDED

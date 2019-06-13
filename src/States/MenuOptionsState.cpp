#include "States/MenuOptionsState.h"
#include <fstream>
#include <iostream>
#include "Core/FileManager.h"
#include "Misc/Utility.h"

MenuOptionsState::MenuOptionsState(GameEngine& game)
    : State(game)
    , m_backgroundSprite(m_game.resourceManager.getTexture("menuBackground"))
    , m_titleText("Options", m_game.resourceManager.getFont("mainFont"), 48)
    , m_soundSliderText("Music Volume", m_game.resourceManager.getFont("mainFont"), 32)
    , m_soundSlider(m_game.resourceManager.getFont("mainFont"), getAbsolutePosition(0.5, 0.33) + sf::Vector2f(50, 0),
                    sf::Vector2f(300, 50), GuiStyle::White, "Sound", 20, -8, 6, 100)
    , m_mustUpdateSoundSettings(false)
{
    // State settings
    m_stateSettings.canSkipUpdates = true;

    // Content settings
    m_backgroundSprite.setOrigin(static_cast<sf::Vector2f>(m_backgroundSprite.getTexture()->getSize()) / 2.0f);
    m_titleText.setOrigin(m_titleText.getLocalBounds().left + m_titleText.getLocalBounds().width / 2,
                          m_titleText.getLocalBounds().top + m_titleText.getLocalBounds().height / 2);
    // Note: Origin is on the right, centered vertically
    m_soundSliderText.setOrigin(m_soundSliderText.getLocalBounds().left + m_soundSliderText.getLocalBounds().width,
                                m_soundSliderText.getLocalBounds().top + m_soundSliderText.getLocalBounds().height / 2);

    // Music settings
    std::ifstream inputFile(FileManager::resourcePath() + "data/settings/sound_settings.txt");
    if (inputFile)
    {
        bool isMuted = false;
        inputFile >> isMuted;
        unsigned int volume = 50;
        inputFile >> volume;
        m_soundSlider.setValue(volume);
        std::cout << "Successfully read sound settings.\n";
    }
    else
    {
        std::cout << "Failed to read sound settings.\n";
    }
}

MenuOptionsState::~MenuOptionsState()
{
}

void MenuOptionsState::handleInput()
{
    if (m_game.inputManager.isKeyDescending(sf::Keyboard::Escape))
    {
        m_game.requestPop();
        return;
    }
    if (m_game.inputManager.detectedMouseMovedEvent())
    {
        m_soundSlider.onMouseHover(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonDescending(sf::Mouse::Left))
    {
        m_soundSlider.onMouseClick(getWindowMousePosition());
    }
    if (m_game.inputManager.isMouseButtonAscending(sf::Mouse::Left))
    {
        if (m_soundSlider.onMouseUnclick(getWindowMousePosition()))
        {
            m_mustUpdateSoundSettings = true;
        }
    }
}

void MenuOptionsState::update()
{
    if (m_mustUpdateSoundSettings == true)
    {
        std::ofstream outputFile(FileManager::resourcePath() + "data/settings/sound_settings.txt");
        if (outputFile)
        {
            if (m_soundSlider.getValue() == 0)
            {
                outputFile << "1 ";
            }
            else
            {
                outputFile << "0 ";
            }
            unsigned int volume = m_soundSlider.getValue();
            outputFile << volume;
            m_mustUpdateSoundSettings = false;
            std::cout << "Successfully wrote sound settings.\n";
        }
        else
        {
            std::cout << "Failed to write sound settings.\n";
        }
    }
}

void MenuOptionsState::draw(sf::RenderTarget& target, float lag)
{
    target.draw(m_backgroundSprite);
    target.draw(m_titleText);

    target.draw(m_soundSliderText);
    target.draw(m_soundSlider);
}

void MenuOptionsState::onWindowResize()
{
    m_backgroundSprite.setPosition(getWindowDimensions() / 2.0f);
    Utility::setSpriteScaleToFill(m_backgroundSprite, getWindowDimensions());
    m_titleText.setPosition(getAbsolutePosition(0.5, 0.2));
    m_soundSlider.setPosition(getAbsolutePosition(0.5, 0.33) + sf::Vector2f(50, 0));
    m_soundSliderText.setPosition(m_soundSlider.getPosition() + sf::Vector2f(-m_soundSlider.getDimensions().x / 2 - 50, 0));
}

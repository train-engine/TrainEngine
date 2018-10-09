#include "MenuOptionsState.h"
#include <fstream>
#include <iostream>
#include "FileManager.h"
#include "Utility.h"

MenuOptionsState::MenuOptionsState(GameEngine& rGame)
    : State(rGame),
      m_backgroundSprite(m_rGame.m_resourceManager.GetTexture("menuBackground")),
      m_titleText("Options", m_rGame.m_resourceManager.GetFont("mainFont"), 48),
      m_soundSliderText("Music Volume", m_rGame.m_resourceManager.GetFont("mainFont"), 32),
      m_soundSlider(m_rGame.m_resourceManager.GetFont("mainFont"),
                    GetAbsolutePosition(0.5, 0.33) + sf::Vector2f(50, 0),
                    sf::Vector2f(300, 50), GuiStyle::White, "Sound", 20, -8, 6, 100),
      m_updateSoundSettings(false)
{
    // State settings
    m_stateSettings.canSkipUpdates = true;

    // Content settings
    m_backgroundSprite.setOrigin(static_cast<sf::Vector2f>(m_backgroundSprite.getTexture()->getSize()) / 2.0f);
    m_titleText.setOrigin(m_titleText.getLocalBounds().left + m_titleText.getLocalBounds().width / 2, m_titleText.getLocalBounds().top + m_titleText.getLocalBounds().height / 2);
    // Note: Origin is on the right, centered vertically
    m_soundSliderText.setOrigin(m_soundSliderText.getLocalBounds().left + m_soundSliderText.getLocalBounds().width, m_soundSliderText.getLocalBounds().top + m_soundSliderText.getLocalBounds().height / 2);

    // Music settings
    std::ifstream inputFile;
    inputFile.open(FileManager::ResourcePath() + "data/settings/sound_settings.txt");
    if (inputFile)
    {
        bool isMuted = 0;
        inputFile >> isMuted;
        unsigned int volume = 50;
        inputFile >> volume;
        m_soundSlider.SetValue(volume);
        inputFile.close();
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

void MenuOptionsState::HandleInput()
{
    if (m_rGame.m_inputManager.KeyDown(sf::Keyboard::Escape))
    {
        m_rGame.RequestPop();
        return;
    }
    if (m_rGame.m_inputManager.DetectMouseMovedEvent())
    {
        m_soundSlider.OnMouseHover(GetWindowMousePosition());
    }
    if (m_rGame.m_inputManager.MouseButtonDown(sf::Mouse::Left))
    {
        m_soundSlider.OnMouseClick(GetWindowMousePosition());
    }
    if (m_rGame.m_inputManager.MouseButtonUp(sf::Mouse::Left))
    {
        if (m_soundSlider.OnMouseUnclick(GetWindowMousePosition()))
        {
            m_updateSoundSettings = true;
        }
    }
}

void MenuOptionsState::Update()
{
    if (m_updateSoundSettings == true)
    {
        std::ofstream outputFile;
        outputFile.open(FileManager::ResourcePath() + "data/settings/sound_settings.txt");
        if (outputFile)
        {
            if (m_soundSlider.GetValue() == 0)
            {
                outputFile << "1 ";
            }
            else
            {
                outputFile << "0 ";
            }
            unsigned int volume = m_soundSlider.GetValue();
            outputFile << volume;
            m_updateSoundSettings = false;
            outputFile.close();
            std::cout << "Successfully wrote sound settings.\n";
        }
        else
        {
            std::cout << "Failed to write sound settings.\n";
        }
    }
}

void MenuOptionsState::Draw(sf::RenderTarget& rTarget, float lag)
{
    rTarget.setView(GetDefaultView());
    
    rTarget.draw(m_backgroundSprite);
    rTarget.draw(m_titleText);

    rTarget.draw(m_soundSliderText);
    rTarget.draw(m_soundSlider);
}

void MenuOptionsState::OnWindowResize()
{
    m_backgroundSprite.setPosition(GetWindowDimensions() / 2.0f);
    Utility::SetSpriteScaleToFill(m_backgroundSprite, GetWindowDimensions());
    m_titleText.setPosition(GetAbsolutePosition(0.5, 0.2));
    m_soundSlider.SetPosition(GetAbsolutePosition(0.5, 0.33) + sf::Vector2f(50, 0));
    m_soundSliderText.setPosition(m_soundSlider.GetPosition() + sf::Vector2f(-m_soundSlider.GetDimensions().x / 2 - 50, 0));
}

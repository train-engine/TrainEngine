#include "States/PauseState.h"

PauseState::PauseState(GameEngine& rGame)
    : State(rGame)
    , m_pausedText("Game Paused", m_rGame.resourceManager.getFont("mainFont"), 96)
    , m_alpha(0)
{
    // Initialize GUI
    const sf::Font& font = m_rGame.resourceManager.getFont("mainFont");
    const sf::SoundBuffer& soundBuffer = m_rGame.resourceManager.getSoundBuffer("click");
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(400, 50), -2, 6, "Back to game", GuiStyle::White);
    m_buttons.emplace_back(font, soundBuffer, sf::Vector2f(0, 0), sf::Vector2f(400, 50), -2, 6, "Exit to main menu", GuiStyle::White);
    for (auto& button : m_buttons)
    {
        button.setVolume(0.75);
    }

    // Content settings
    m_pausedText.setOrigin(m_pausedText.getLocalBounds().left + m_pausedText.getLocalBounds().width / 2,
                           m_pausedText.getLocalBounds().top + m_pausedText.getLocalBounds().height / 2);
}

PauseState::~PauseState()
{
}

void PauseState::handleInput()
{
    if (m_rGame.inputManager.isKeyDescending(sf::Keyboard::Escape))
    {
        m_rGame.requestPop();
        return;
    }
    if (m_rGame.inputManager.detectedMouseMovedEvent())
    {
        for (auto& rButton : m_buttons)
        {
            rButton.onMouseHover(getWindowMousePosition());
        }
    }
    if (m_rGame.inputManager.isMouseButtonDescending(sf::Mouse::Left))
    {
        for (auto& rButton : m_buttons)
        {
            rButton.onMouseClick(getWindowMousePosition());
        }
    }
    if (m_rGame.inputManager.isMouseButtonAscending(sf::Mouse::Left))
    {
        for (std::size_t i = 0; i < m_buttons.size(); i++)
        {
            if (m_buttons[i].onMouseUnclick(getWindowMousePosition()) == true)
            {
                switch (i)
                {
                // Back to game button
                case 0:
                    m_rGame.requestPop();
                    return;
                // Exit to main menu button
                case 1:
                    m_rGame.requestPop(2);
                    return;
                default:
                    break;
                }
                break; // Break because if one button has been clicked, the others cannot have also been clicked (optimization)
            }
        }
    }
}

void PauseState::update()
{
    if (m_alpha < 200)
    {
        m_alpha += 10;
        m_stateSettings.backgroundColor = sf::Color(200, 200, 200, m_alpha);
    }
}

void PauseState::draw(sf::RenderTarget& rTarget, float lag)
{
    m_rGame.drawPreviousState(this);

    drawBackgroundColor(rTarget);
    rTarget.draw(m_pausedText);

    for (const auto& button : m_buttons)
    {
        rTarget.draw(button);
    }
}

void PauseState::onWindowResize()
{
    m_pausedText.setPosition(getAbsolutePosition(0.5, 0.25));
    for (std::size_t i = 0; i < m_buttons.size(); i++)
    {
        m_buttons[i].setPosition(m_pausedText.getPosition() + sf::Vector2f(0, 150 + 70 * i));
    }
}

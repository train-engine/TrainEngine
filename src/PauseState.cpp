#include "PauseState.h"
#include <iostream>

PauseState::PauseState(GameEngine& rGame)
    : State(rGame),
      m_pausedText("Game Paused", m_rGame.m_resourceManager.GetFont("mainFont"), 96),
      m_alpha(0)
{
    // Initialize GUI
    m_buttons.emplace_back(m_rGame.m_resourceManager.GetFont("mainFont"), m_rGame.m_resourceManager.GetSoundBuffer("click"),
                           sf::Vector2f(0, 0),
                           sf::Vector2f(400, 50), -2, 6, "Back to game", GuiStyle::White);
    m_buttons.emplace_back(m_rGame.m_resourceManager.GetFont("mainFont"), m_rGame.m_resourceManager.GetSoundBuffer("click"),
                           sf::Vector2f(0, 0),
                           sf::Vector2f(400, 50), -2, 6, "Exit to main menu", GuiStyle::White);
    for (auto& button : m_buttons)
    {
        button.SetVolume(0.75);
    }

    // Content settings
    m_pausedText.setOrigin(m_pausedText.getLocalBounds().left + m_pausedText.getLocalBounds().width / 2, m_pausedText.getLocalBounds().top + m_pausedText.getLocalBounds().height / 2);
}

PauseState::~PauseState()
{

}

void PauseState::HandleInput()
{
    if (m_rGame.m_inputManager.IsKeyDescending(sf::Keyboard::Escape))
    {
        m_rGame.RequestPop();
        return;
    }
    if (m_rGame.m_inputManager.DetectedMouseMovedEvent())
    {
        for (auto& rButton : m_buttons)
        {
            rButton.OnMouseHover(GetWindowMousePosition());
        }
    }
    if (m_rGame.m_inputManager.IsMouseButtonDescending(sf::Mouse::Left))
    {
        for (auto& rButton : m_buttons)
        {
            rButton.OnMouseClick(GetWindowMousePosition());
        }
    }
    if (m_rGame.m_inputManager.IsMouseButtonAscending(sf::Mouse::Left))
    {
        for (unsigned int i = 0; i < m_buttons.size(); i++)
        {
            if (m_buttons[i].OnMouseUnclick(GetWindowMousePosition()) == true)
            {
                switch (i)
                {
                // Back to game button
                case 0:
                    m_rGame.RequestPop();
                    return;
                // Exit to main menu button
                case 1:
                    m_rGame.RequestPop(2);
                    return;
                default:
                    break;
                }
                break; // Because if one button has been clicked, the others cannot possibly be clicked at the same time (optimization)
            }
        }
    }
}

void PauseState::Update()
{
    if (m_alpha < 200)
    {
        m_alpha += 10;
        SetBackgroundColor(sf::Color(200, 200, 200, m_alpha));
    }
}

void PauseState::Draw(sf::RenderTarget& rTarget, float lag)
{
    m_rGame.DrawPreviousState(this);

    DrawBackgroundColor(rTarget);
    rTarget.draw(m_pausedText);

    for (const auto& button : m_buttons)
    {
        rTarget.draw(button);
    }
}

void PauseState::OnWindowResize()
{
    m_pausedText.setPosition(GetAbsolutePosition(0.5, 0.25));
    for (unsigned int i = 0; i < m_buttons.size(); i++)
    {
        m_buttons[i].SetPosition(m_pausedText.getPosition() + sf::Vector2f(0, 150 + 70 * i));
    }
}

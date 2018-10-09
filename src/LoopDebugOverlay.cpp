#include "LoopDebugOverlay.h"
#include <string>
#include "GameEngine.h"

namespace
{
    const sf::Time samplingTime = sf::milliseconds(250);
}

LoopDebugOverlay::LoopDebugOverlay(const sf::Font& font)
    : m_upsText("UPS: ", font, 15),
      m_fpsText("FPS: ", font, 15),
      m_updateStrainText("UPS Strain: ", font, 15),
      m_drawStrainText("FPS Strain: ", font, 15),
      m_recordedUps(0),
      m_recordedFps(0),
      m_updateCounter(0),
      m_drawCounter(0),
      m_isVisible(false)
{
    m_upsText.setFillColor(sf::Color::White);
    m_upsText.setOutlineColor(sf::Color(50, 50, 50));
    m_upsText.setOutlineThickness(1);
    
    m_fpsText.setFillColor(sf::Color::White);
    m_fpsText.setOutlineColor(sf::Color(50, 50, 50));
    m_fpsText.setOutlineThickness(1);
    
    m_updateStrainText.setFillColor(sf::Color::White);
    m_updateStrainText.setOutlineColor(sf::Color(50, 50, 50));
    m_updateStrainText.setOutlineThickness(1);
    
    m_drawStrainText.setFillColor(sf::Color::White);
    m_drawStrainText.setOutlineColor(sf::Color(50, 50, 50));
    m_drawStrainText.setOutlineThickness(1);
    
    OnWindowResize();
}

void LoopDebugOverlay::draw(sf::RenderTarget& rTarget, sf::RenderStates states) const
{
    if (m_isVisible == true)
    {
        rTarget.draw(m_upsText);
        rTarget.draw(m_fpsText);
        rTarget.draw(m_updateStrainText);
        rTarget.draw(m_drawStrainText);
    }
}

void LoopDebugOverlay::RecordUpdate(sf::Time lastUpdateTime)
{
    m_updateCounter++;
    m_sampledUpdateTime += lastUpdateTime;
    if (m_upsClock.getElapsedTime() >= samplingTime) // Only recalculate after a set amount of time
    {
        m_recordedUps = 1000000 / static_cast<double>(m_upsClock.restart().asMicroseconds()) * m_updateCounter;
        
        std::string tempString = std::to_string(m_recordedUps);
        tempString.erase(tempString.end() - 3, tempString.end());
        m_upsText.setString("UPS: " + tempString);

        tempString = std::to_string(static_cast<double>(m_sampledUpdateTime.asMicroseconds()) / m_updateCounter * m_recordedUps / 1000000 * 100);
        tempString.erase(tempString.end() - 3, tempString.end());
        m_updateStrainText.setString("UPS Strain: " + tempString + "%");
        
        m_sampledUpdateTime = sf::Time::Zero;
        m_updateCounter = 0;
    }
}

void LoopDebugOverlay::RecordDraw(sf::Time lastDrawTime)
{
    m_drawCounter++;
    m_sampledDrawTime += lastDrawTime;
    if (m_fpsClock.getElapsedTime() >= samplingTime) // Only recalculate after a set amount of time
    {
        m_recordedFps = 1000000 / static_cast<double>(m_fpsClock.restart().asMicroseconds()) * m_drawCounter;
        
        std::string tempString = std::to_string(m_recordedFps);
        tempString.erase(tempString.end() - 3, tempString.end());
        m_fpsText.setString("FPS: " + tempString);
        
        tempString = std::to_string(static_cast<double>(m_sampledDrawTime.asMicroseconds()) / m_drawCounter * m_recordedFps / 1000000 * 100);
        tempString.erase(tempString.end() - 3, tempString.end());
        m_drawStrainText.setString("FPS Strain: " + tempString + "%");
        
        m_sampledDrawTime = sf::Time::Zero;
        m_drawCounter = 0;
    }
}

void LoopDebugOverlay::OnWindowResize()
{
    m_upsText.setPosition(5, 5);
    m_updateStrainText.setPosition(m_upsText.getPosition().x, m_upsText.getGlobalBounds().top + m_upsText.getFont()->getLineSpacing(m_upsText.getCharacterSize()));
    m_fpsText.setPosition(m_updateStrainText.getPosition().x, m_updateStrainText.getGlobalBounds().top + m_updateStrainText.getFont()->getLineSpacing(m_updateStrainText.getCharacterSize()));
    m_drawStrainText.setPosition(m_fpsText.getPosition().x, m_fpsText.getGlobalBounds().top + m_fpsText.getFont()->getLineSpacing(m_fpsText.getCharacterSize()));
}

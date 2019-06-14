#include "Level/EntityTracker.h"
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Core/FileManager.h"

EntityTracker::EntityTracker(const sf::Font& font)
    : m_trackedEntity(nullptr)
    , m_dot(3)
    , m_lastPositionText("", font, 16)
    , m_lastVelocityText("", font, 16)
    , m_distanceTraveledText("", font, 16)
    , m_displacementText("", font, 16)
    , m_positionsCountText("", font, 16)
    , m_totalDistanceTraveled(0)
    , m_isDotPathVisible(false)
    , m_isInfoBoxVisible(false)
{
    m_dot.setFillColor(sf::Color::Green);

    m_textContainer.setFillColor(sf::Color(255, 255, 255, 225));
    m_textContainer.setOutlineThickness(1);
    m_textContainer.setOutlineColor(sf::Color(50, 50, 50, 200));

    m_lastPositionText.setFillColor(sf::Color::Black);
    m_lastVelocityText.setFillColor(sf::Color::Black);
    m_distanceTraveledText.setFillColor(sf::Color::Black);
    m_displacementText.setFillColor(sf::Color::Black);
    m_positionsCountText.setFillColor(sf::Color::Black);

    updateInfoBox();
}

void EntityTracker::updateInfoBox()
{
    sf::Vector2f lastPosition;
    if (m_positions.empty())
    {
        lastPosition = sf::Vector2f(0, 0);
    }
    else
    {
        lastPosition = m_positions.back();
    }

    std::string tempString1 = std::to_string(lastPosition.x);
    tempString1.erase(tempString1.end() - 3, tempString1.end());
    std::string tempString2 = std::to_string(lastPosition.y);
    tempString2.erase(tempString2.end() - 3, tempString2.end());
    m_lastPositionText.setString("Last position: (" + tempString1 + ", " + tempString2 + ")");

    if (m_positions.size() > 1)
    {
        tempString1 = std::to_string(m_positions.back().x - m_positions.end()[-2].x);
        tempString2 = std::to_string(m_positions.back().y - m_positions.end()[-2].y);
    }
    else
    {
        tempString1 = std::to_string(0.0f);
        tempString2 = std::to_string(0.0f);
    }
    tempString1.erase(tempString1.end() - 3, tempString1.end());
    tempString2.erase(tempString2.end() - 3, tempString2.end());
    m_lastVelocityText.setString("Last velocity: (" + tempString1 + ", " + tempString2 + ")");

    tempString1 = std::to_string(getDistanceTraveled());
    tempString1.erase(tempString1.end() - 3, tempString1.end());
    m_distanceTraveledText.setString("Distance traveled: " + tempString1);

    tempString1 = std::to_string(getDisplacement());
    tempString1.erase(tempString1.end() - 3, tempString1.end());
    m_displacementText.setString("Displacement: " + tempString1);

    m_positionsCountText.setString("Number of points: " + std::to_string(m_positions.size()));

    m_lastPositionText.setPosition(lastPosition + sf::Vector2f(25, 25));
    m_lastVelocityText.setPosition(m_lastPositionText.getPosition().x,
                                   m_lastPositionText.getGlobalBounds().top +
                                       m_lastPositionText.getFont()->getLineSpacing(m_lastPositionText.getCharacterSize()));
    m_distanceTraveledText.setPosition(m_lastVelocityText.getPosition().x,
                                       m_lastVelocityText.getGlobalBounds().top +
                                           m_lastVelocityText.getFont()->getLineSpacing(m_lastVelocityText.getCharacterSize()));
    m_displacementText.setPosition(m_distanceTraveledText.getPosition().x,
                                   m_distanceTraveledText.getGlobalBounds().top +
                                       m_distanceTraveledText.getFont()->getLineSpacing(m_distanceTraveledText.getCharacterSize()));
    m_positionsCountText.setPosition(m_displacementText.getPosition().x,
                                     m_displacementText.getGlobalBounds().top +
                                         m_displacementText.getFont()->getLineSpacing(m_displacementText.getCharacterSize()));

    float biggestTextWidth =
        std::fmax(std::fmax(m_lastPositionText.getGlobalBounds().width, m_distanceTraveledText.getGlobalBounds().width),
                  std::fmax(m_displacementText.getGlobalBounds().width, m_positionsCountText.getGlobalBounds().width));
    float padding = 2;

    m_textContainer.setSize(sf::Vector2f(biggestTextWidth + padding * 2,
                                         m_positionsCountText.getGlobalBounds().top + m_positionsCountText.getCharacterSize() -
                                             m_lastPositionText.getGlobalBounds().top + padding * 2));
    m_textContainer.setPosition(m_lastPositionText.getGlobalBounds().left - padding, m_lastPositionText.getGlobalBounds().top - padding);
}

void EntityTracker::update()
{
    if (m_trackedEntity != nullptr)
    {
        // Verify if the position is not the same as the previous one,
        // to avoid useless duplicate positions in the vector
        if (m_positions.empty() || m_positions.back() != m_trackedEntity->getPosition())
        {
            m_positions.push_back(m_trackedEntity->getPosition());
            if (m_positions.size() > 1)
            {
                m_totalDistanceTraveled +=
                    hypot(m_positions.back().x - m_positions.end()[-2].x, m_positions.back().y - m_positions.end()[-2].y);
            }
        }

        if (m_isInfoBoxVisible == true)
        {
            updateInfoBox();
        }
    }
}

void EntityTracker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_isDotPathVisible == true)
    {
        for (const auto& position : m_positions)
        {
            if (position.x >= target.getView().getCenter().x - target.getView().getSize().x / 2 - m_dot.getRadius() &&
                position.x <= target.getView().getCenter().x + target.getView().getSize().x / 2 + m_dot.getRadius() &&
                position.y >= target.getView().getCenter().y - target.getView().getSize().y / 2 - m_dot.getRadius() &&
                position.y <= target.getView().getCenter().y + target.getView().getSize().y / 2 + m_dot.getRadius())
            {
                m_dot.setPosition(position);
                target.draw(m_dot, states);
            }
        }
    }
    if (m_isInfoBoxVisible == true)
    {
        target.draw(m_textContainer, states);
        target.draw(m_lastPositionText, states);
        target.draw(m_lastVelocityText, states);
        target.draw(m_distanceTraveledText, states);
        target.draw(m_displacementText, states);
        target.draw(m_positionsCountText, states);
    }
}

void EntityTracker::resetTracking()
{
    m_positions.clear();
    m_totalDistanceTraveled = 0;
    updateInfoBox();
}

void EntityTracker::stopTracking()
{
    m_trackedEntity = nullptr;
}

void EntityTracker::outputToExcel() const
{
    std::time_t t = std::time(nullptr);
    std::tm* time = std::localtime(&t);
    std::ostringstream filenameStream;
    filenameStream << 1900 + time->tm_year << 1 + time->tm_mon << time->tm_mday << 1 + time->tm_hour - time->tm_isdst << 1 + time->tm_min
                   << 1 + time->tm_sec;

    std::ofstream outputFile(FileManager::resourcePath() + "logs/tracker_" + filenameStream.str() + ".csv");
    if (outputFile)
    {
        for (const auto& position : m_positions)
        {
            outputFile << position.x << "," << position.y << '\n';
        }

        std::cout << "EntityTracker: Data successfully output to Excel format file.\n";
    }
    else
    {
        std::cerr << "EntityTracker error: Unable to save to Excel format file.\n";
    }
}

float EntityTracker::getDisplacement() const
{
    if (m_positions.size() > 1)
    {
        return std::hypot(m_positions.back().x - m_positions.front().x, m_positions.back().y - m_positions.front().y);
    }

    return 0;
}

#ifndef LOOPDEBUGOVERLAY_H
#define LOOPDEBUGOVERLAY_H

#include <SFML/Graphics.hpp>

// Class used for displaying debug information relating to the game loop (UPS, FPS, UPS strain, and FPS strain)

class GameEngine;

class LoopDebugOverlay final : public sf::Drawable
{
private:
    sf::Text m_upsText;
    sf::Text m_fpsText;
    sf::Text m_updateStrainText;
    sf::Text m_drawStrainText;

    sf::Clock m_upsClock;
    sf::Clock m_fpsClock;
    sf::Time m_sampledUpdateTime;
    sf::Time m_sampledDrawTime;

    double m_recordedUps;
    double m_recordedFps;

    unsigned int m_updateCounter;
    unsigned int m_drawCounter;

    bool m_isVisible;

    // Functions
    virtual void draw(sf::RenderTarget& rTarget, sf::RenderStates states) const override;

public:
    // Constructor
    explicit LoopDebugOverlay(const sf::Font& font);

    // Functions
    void RecordUpdate(sf::Time lastUpdateTime);
    void RecordDraw(sf::Time lastDrawTime);
    void OnWindowResize();

    // Setters
    void ToggleVisible() { m_isVisible = !m_isVisible; }

    // Getters
    double GetRecordedUps() const { return m_recordedUps; }
    double GetRecordedFps() const { return m_recordedFps; }
};

#endif // LOOPDEBUGOVERLAY_H

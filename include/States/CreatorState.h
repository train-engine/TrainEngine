#ifndef CREATORSTATE_H
#define CREATORSTATE_H

#include "Gui/Gui.h"
#include "Level/Level.h"
#include "States/State.h"
#include "Gui/TextBox.h"

class CreatorState final : public State
{
private:
    sf::RectangleShape m_panel;

    sf::Text m_loadLevelLabel;
    sf::Text m_saveLevelLabel;
    sf::Text m_tileNameLabel;

    TextBox m_loadLevelTextBox;
    TextBox m_saveLevelTextBox;
    TextBox m_widthTextBox;
    TextBox m_heightTextBox;
    TextBox m_tileNameTextBox;

    GuiRectSoundButton m_createLevelButton;
    Level m_level;

    std::array<TileType, 5> m_selectableTileTypes;
    unsigned int m_selectedTileTypeIndex;

    sf::Vector2f m_currentMousePosition;
    sf::Vector2f m_previousMousePosition;

    int m_musicNumber;
    sf::Music m_music;

    unsigned int m_brushSize;

    // Destructor
    virtual ~CreatorState() override;

    // Functions
    void SetMusic();
    void PauseStart();
    sf::Vector2u GetBrushTopLeftIndex(const sf::Vector2f& mousePosition);
    void ClearPreviewMap();
    bool CheckMouseChangedTile();

    // State functions
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Draw(sf::RenderTarget& rTarget, float lag) override;

    virtual void OnWindowResize() override;

public:
    // Constructors
    explicit CreatorState(GameEngine& rGame);
    CreatorState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // CREATORSTATE_H

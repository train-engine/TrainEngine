#ifndef CREATORSTATE_H_INCLUDED
#define CREATORSTATE_H_INCLUDED

#include "Gui.h"
#include "Level.h"
#include "State.h"
#include "TextBox.h"

class CreatorState : public State
{
private:
    sf::RectangleShape m_panel;

    sf::Text m_loadText;
    sf::Text m_saveText;
    sf::Text m_tileText;

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

    // Functions
    void SetMusic();
    void PauseStart();
    sf::Vector2u GetBrushTopLeftIndex(const sf::Vector2f& mousePosition);
    void ClearPreviewMap();
    bool CheckMouseChangedTile();

protected:
    // Destructor
    ~CreatorState() override;

    // Functions
    void HandleInput() override;
    void Update() override;
    void Draw(sf::RenderTarget& rTarget, float lag) override;

    void OnWindowResize() override;

public:
    // Constructors
    CreatorState(GameEngine& rGame);
    CreatorState(GameEngine& rGame, const std::string& levelDirectory);
};

#endif // CREATORSTATE_H_INCLUDED

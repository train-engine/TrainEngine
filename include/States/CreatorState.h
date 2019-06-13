#ifndef CREATORSTATE_H
#define CREATORSTATE_H

#include "Gui/Gui.h"
#include "Gui/TextBox.h"
#include "Level/Level.h"
#include "States/State.h"

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
    void setMusic();
    void pauseStart();
    sf::Vector2u getBrushTopLeftIndex(const sf::Vector2f& mousePosition);
    void clearPreviewMap();
    bool checkMouseChangedTile();

    // State functions
    virtual void handleInput() override;
    virtual void update() override;
    virtual void draw(sf::RenderTarget& target, float lag) override;

    virtual void onWindowResize() override;

public:
    // Constructors
    explicit CreatorState(GameEngine& game);
    CreatorState(GameEngine& game, const std::string& levelDirectory);
};

#endif // CREATORSTATE_H

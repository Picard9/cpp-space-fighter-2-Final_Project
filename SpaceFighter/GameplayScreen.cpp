#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "SelectAircraftScreen.h"
#include "Level.h"
#include "Level01.h"
#include "PlayerShip.h"  
#include "Blaster.h"     
#include "Projectile.h"     
#include "GameOver.h"
#include "Victory.h"
#include "Level02.h"

GameplayScreen::GameplayScreen(AircraftType aircraftType)
    : m_levelIndex(0),
    m_pLevel(nullptr),
    m_pResourceManager(nullptr),
    m_aircraftType(aircraftType)
{
    SetTransitionInTime(1.0f);
    SetTransitionOutTime(3.0f);

    // Return to aircraft selection when this screen is removed
    SetOnRemove([this]() { AddScreen(new SelectAircraftScreen()); });

    Show();
}



void GameplayScreen::LoadContent(ResourceManager& resourceManager)
{
    m_pResourceManager = &resourceManager;
    LoadLevel(m_levelIndex);
}

void GameplayScreen::LoadLevel(const int levelIndex)
{
    if (m_pLevel) delete m_pLevel;

    switch (levelIndex)
    {
    case 0: m_pLevel = new Level01(m_aircraftType); break;  // pass the selected aircraft type
    case 1: m_pLevel = new Level02(m_aircraftType); break;  // add Level02 support
    default:
        m_pLevel = new Level01(m_aircraftType); break; // fallback
    }

    m_pLevel->SetGameplayScreen(this);
    m_pLevel->LoadContent(*m_pResourceManager);
}

void GameplayScreen::HandleInput(const InputState& input)
{
    m_pLevel->HandleInput(input);
}

void GameplayScreen::Update(const GameTime& gameTime) // updated to handle level transitions and game over/victory conditions -- paul
{
    if (!m_pLevel) return;

    // update the current level
    m_pLevel->Update(gameTime);

    // check if player died
    if (!m_gameEnded && !m_pLevel->GetPlayerShip()->IsActive())
    {
        m_gameEnded = true;
        SetOnRemove([this]() { AddScreen(new GameOverScreen()); });
        Exit();
        return;
    }

    // check if level is complete
    if (m_pLevel->IsComplete() && !m_gameEnded)
    {
        // move to the next level
        m_levelIndex++;

        // number of last level index (update if you add more levels)
        const int MAX_LEVEL_INDEX = 1;

        // if we've passed the last level, show victory
        if (m_levelIndex > MAX_LEVEL_INDEX)
        {
            m_gameEnded = true;
            SetOnRemove([this]() { AddScreen(new VictoryScreen()); });
            Exit();
            return;
        }

        // otherwise, load next level using LoadLevel (it will delete previous level)
        std::cout << "Level complete! Switching to Level " << (m_levelIndex + 1) << "\n";
        LoadLevel(m_levelIndex);
    }
}



void GameplayScreen::Draw(SpriteBatch& spriteBatch)
{
    spriteBatch.Begin();

    m_pLevel->Draw(spriteBatch);

    spriteBatch.End();
}

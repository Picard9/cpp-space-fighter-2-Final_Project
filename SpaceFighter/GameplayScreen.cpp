#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "SelectAircraftScreen.h"
#include "Level.h"
#include "Level01.h"
#include "PlayerShip.h"  
#include "Blaster.h"     
#include "Projectile.h"     



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

    // Ensure a level exists first
    if (!m_pLevel)
        LoadLevel(m_levelIndex);

    // Always create a player ship
    PlayerShip* pPlayer = new PlayerShip(m_aircraftType);
    // Attach Blaster so GetWeapon("Main Blaster") is never nullptr
    Blaster* pBlaster = new Blaster("Main Blaster");

    // If you later add projectile pool in Level, uncomment this
    pBlaster->SetProjectilePool(&m_pLevel->GetProjectiles());
    pPlayer->AttachItem(pBlaster, Vector2::UNIT_Y * -20);


    // Load textures, sounds, etc.
    pPlayer->LoadContent(resourceManager);
    pPlayer->Activate();

    m_pLevel->SetPlayerShip(pPlayer); //Give player to the level




    // Add PlayerShip to level
    m_pLevel->AddGameObject(pPlayer);


}



void GameplayScreen::LoadLevel(const int levelIndex)
{
	if (m_pLevel) delete m_pLevel;

	switch (levelIndex)
	{
	    case 0: m_pLevel = new Level01(m_aircraftType); break;  // pass the selected aircraft type
	}

	m_pLevel->SetGameplayScreen(this);
	m_pLevel->LoadContent(*m_pResourceManager);
}

void GameplayScreen::HandleInput(const InputState& input)
{
	m_pLevel->HandleInput(input);
}

void GameplayScreen::Update(const GameTime& gameTime)
{
	m_pLevel->Update(gameTime);
}

void GameplayScreen::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();

	m_pLevel->Draw(spriteBatch);

	spriteBatch.End();
}

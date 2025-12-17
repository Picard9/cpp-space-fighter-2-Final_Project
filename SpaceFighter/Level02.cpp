#include "Level02.h"
#include "BioEnemyShip.h"
#include "EnemyShipMedium.h"
#include "EnemyShipLarge.h"
#include "PlayerShip.h"
#include <iostream>

Level02::Level02(AircraftType aircraftType)
    : Level(aircraftType)
{
}

void Level02::LoadContent(ResourceManager& resourceManager)
{
    std::cout << "[Level02] LoadContent started.\n";

    m_hasHadActiveEnemy = false;
    m_bossAlive = true;

    // --------------------------
    // Spawn small/bio enemies
    // --------------------------
    Texture* pBioTex = resourceManager.Load<Texture>("Textures\\BioEnemyShip.png");
    float delay = 0.0f;
    Vector2 position;

    const int BIO_COUNT = 22;
    for (int i = 0; i < BIO_COUNT; i++)
    {
        delay += 0.25f;
        position.Set(0.25f * Game::GetScreenWidth(), -pBioTex->GetCenter().Y);

        BioEnemyShip* pEnemy = new BioEnemyShip();
        pEnemy->SetTexture(pBioTex);
        pEnemy->SetCurrentLevel(this);
        pEnemy->Initialize(position, delay);
        AddGameObject(pEnemy);

        pEnemy->SetDelaySeconds(delay);
    }

    // --------------------------
    // Spawn medium enemies
    // --------------------------
    Texture* pMediumTex = resourceManager.Load<Texture>("Textures\\EnemyShipMedium.png");
    for (int i = 0; i < 5; i++)
    {
        float enemyDelay = 4.0f + i;
        EnemyShipMedium* pMedium = new EnemyShipMedium();
        pMedium->SetTexture(pMediumTex);
        pMedium->SetCurrentLevel(this);
        pMedium->Initialize(Vector2(200 + i * 120, -50), enemyDelay);
        AddGameObject(pMedium);

        pMedium->SetDelaySeconds(enemyDelay);
    }

    // --------------------------
    // Spawn boss (large enemy)
    // --------------------------
    Texture* pBossTex = resourceManager.Load<Texture>("Textures\\BioEnemyBoss.png");
    EnemyShipLarge* pBoss = new EnemyShipLarge();
    pBoss->SetTexture(pBossTex);
    pBoss->SetCurrentLevel(this);
    pBoss->Initialize(Vector2(Game::GetScreenWidth() / 2, pBossTex->GetCenter().Y + 40), 0.0f);
    pBoss->Activate();
    AddGameObject(pBoss);

    // When boss is destroyed, mark m_bossAlive = false
    pBoss->SetOnDestroy([this]() { m_bossAlive = false; });

    // --------------------------
    // Set background
    // --------------------------
    SetBackground(resourceManager.Load<Texture>("Textures\\SpaceBackground02.png"));

    std::cout << "[Level02] LoadContent finished.\n";

    Level::LoadContent(resourceManager);
}

void Level02::Update(const GameTime& gameTime)
{
    Level::Update(gameTime);

    // Count active enemies (including boss)
    int activeCount = 0;
    for (GameObject* obj : GetGameObjects())
    {
        if (obj->IsActive() && obj->HasMask(CollisionType::Enemy))
            activeCount++;
    }

    // Track if we ever had any active enemy
    if (activeCount > 0)
        m_hasHadActiveEnemy = true;

    // Level is considered complete if boss is destroyed and we had active enemies
    bool levelComplete = !m_bossAlive && m_hasHadActiveEnemy;

    // Debug output
    std::cout << "[Level02] Active enemies: " << activeCount
        << ", Boss alive: " << m_bossAlive
        << ", HasH

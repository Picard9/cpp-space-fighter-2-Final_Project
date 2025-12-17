#pragma once
#include "Level.h"

/** @brief The second level of the game. */
class Level02 : public Level
{
public:
    Level02(AircraftType type);

    virtual void LoadContent(ResourceManager& resourceManager) override;
    virtual void Update(const GameTime& gameTime) override;

protected:
    bool m_bossAlive = true;
    bool m_hasHadActiveEnemy = false;
};

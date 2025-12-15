#include "SingleShot.h"
#include "Level.h"
#include <cmath>


// The key is used to identify this weapon(like "Main Blaster")
SingleShot::SingleShot(const std::string& key)
    : Weapon(key)
{
}

//This controls how fast the weapon can fire
void SingleShot::Update(const GameTime& gameTime)
{
    if (m_cooldown > 0)
        m_cooldown -= gameTime.GetElapsedTime();
}

//Weapon can fire only when cooldown is finished
bool SingleShot::CanFire() const
{
    return m_cooldown <= 0;
}

//This Prevents the weapon from firing again immediately
void SingleShot::ResetCooldown()
{
    m_cooldown = m_cooldownSeconds;
}

//Fire is called when the player presses the fire button
void SingleShot::Fire(TriggerType triggerType)
{
    if (triggerType != GetTriggerType() || !CanFire() || !IsActive())
        return;

    Projectile* p = GetProjectile();
    if (!p) return;

    Vector2 spawnOffset(0, -25);     // gun barrel (front of the ship)

    // Straight laser shot (NO angle, NO spread)
    Vector2 velocity(0.0f, -6000.0f);   // Moves straight up very fast

    p->Activate(GetPosition() + spawnOffset, true, velocity);

    ResetCooldown();

    // Play laser sound if available
    if (GetFireSound())
        GetFireSound()->Play();
}

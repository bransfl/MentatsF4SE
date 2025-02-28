#pragma once

// unequips and reequips the player's grenade on save load to fix a semi-common movement speed bug
namespace Internal::Fixes::GrenadeMovementSpeedFix
{
	void Install() noexcept;

	void Fix();
}

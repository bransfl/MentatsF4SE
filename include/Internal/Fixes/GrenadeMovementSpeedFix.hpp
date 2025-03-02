#pragma once

// unequips and reequips the player's grenade on save load to fix a semi-common movement speed bug
namespace Internal::Fixes
{
	class GrenadeMovementSpeedFix
	{
	public:
		// installs the fix
		static void Install() noexcept;

		// reequips the player's grenade to reset movement speed
		static void Fix();
	};
}

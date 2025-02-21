#pragma once

#include <RE/Bethesda/ActiveEffect.hpp>

namespace Internal::Fixes::MagicEffectConditions
{
	void Install() noexcept;

	static float ActiveEffectConditionUpdateInterval();
	
	static void Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate);
}

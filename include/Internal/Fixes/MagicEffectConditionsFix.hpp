#pragma once

namespace Internal::Fixes::MagicEffectConditionsFix
{
	void Install() noexcept;

	static float ActiveEffectConditionUpdateInterval();

	static void Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate);
}

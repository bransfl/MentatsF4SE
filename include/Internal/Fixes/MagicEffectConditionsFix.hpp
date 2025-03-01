#pragma once

namespace Internal::Fixes
{
	class MagicEffectConditionsFix
	{
	public:
		static void Install() noexcept;

	private:
		static float ActiveEffectConditionUpdateInterval();
		static void Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate);
	};
}

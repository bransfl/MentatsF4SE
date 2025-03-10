#pragma once

namespace Internal::Fixes
{
	class MagicEffectConditionsFix
	{
	public:
		// installs the fix
		static void Install() noexcept;

	private:
		// returns the value of GameSetting fActiveEffectConditionUpdateInterval
		static float ActiveEffectConditionUpdateInterval();

		// the hook
		static void Hook_EvaluateConditions(RE::ActiveEffect* a_activeEffect, float a_elapsedTimeDelta, bool a_forceUpdate);
	};
}

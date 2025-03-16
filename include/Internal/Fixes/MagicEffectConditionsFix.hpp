#pragma once

namespace Internal::Fixes
{
	class MagicEffectConditionsFix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Gets the value of GameSetting fActiveEffectConditionUpdateInterval, providing an alternate value if the setting is invalid.
		 * @return The value of fActiveEffectConditionUpdateInterval
		 */
		static float ActiveEffectConditionUpdateInterval();

		/**
		 * @brief Hook for ActiveEffect::EvaluateConditions.
		 * @details This hook stores extra timer data on RE::ActiveEffects's unused pad94 member to minimize floating point precision issues.
		 * @param a_this The ActiveEffect that is being evaluated.
		 * @param a_elapsedTimeDelta The amount of time that this effect has been active.
		 * @param a_forceUpdate If this condition should be forcefully re-evaluated. (?)
		 */
		static void Hook_EvaluateConditions(RE::ActiveEffect* a_this, float a_elapsedTimeDelta, bool a_forceUpdate);
	};
}

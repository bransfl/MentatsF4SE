#pragma once

namespace Internal::Fixes::MagicEffectConditions
{
	void Install() noexcept;
	class MagicEffectConditions
	{
	public:
		static void Fix(bool& magicEffectConditions);

	private:
		static float ActiveEffectConditionUpdateInterval();
		//static void  EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate);
	};
}

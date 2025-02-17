#include "Internal/Config/Config.hpp"
#include "RE/Bethesda/Settings.hpp"
#include <RE/Bethesda/Actor.hpp>
#include <string>

namespace Internal::Fixes::MagicEffectConditions
{
	void Install() noexcept
	{
		if (!Config::bMagicEffectConditions.GetValue()) {
			logger::info("MagicEffectConditions -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}

		if (std::filesystem::exists("Data/F4SE/Plugins/MGEFConditionFix.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - MGEF Condition Fix was detected. It is recommended that you disable this mod while using EngineFixesF4SE.");
			logger::info("MagicEffectConditions -> MGEFConditionFix was installed. Aborting fix.");
			return;
		}

		// should have the contents of Fix() here probably
	}

	void Fix(bool& magicEffectConditions)
	{
		// note - bool& magicEffectConditions isnt needed, in SSE engine fixes it's just a config option for applying the patch

		if (magicEffectConditions == true) {
			// temp to make the compiler chill
		}
		// Utility::Memory::SafeWriteAbsoluteJump(Addresses::Fixes::MagicEffectConditions::EvaluateConditions, reinterpret_cast<std::uintptr_t>(std::addressof(MagicEffectConditions::EvaluateConditions)));
	}

	float ActiveEffectConditionUpdateInterval()
	{
		static RE::Setting* conditionInterval = RE::GameSettingCollection::GetSingleton()->GetSetting("fActiveEffectConditionUpdateInterval");
		if (conditionInterval->GetFloat() > 0.001F) {
			return conditionInterval->GetFloat();
		}
		else {
			return 1.0F;
		}
	}

	void EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
	{
		if (activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was kNotAvailable. Return.");
			return;
		}

		if ((activeEffect->flags.all(RE::ActiveEffect::Flags::kHasConditions) || activeEffect->displacementSpell) && activeEffect->target && activeEffect->target->GetTargetStatsObject()) {

			auto& conditionUpdateTime = reinterpret_cast<float&>(activeEffect->pad94);

			if (!forceUpdate) {
				if (activeEffect->elapsedSeconds <= 0.0F) {
					reinterpret_cast<float&>(activeEffect->pad94) = elapsedTimeDelta;
					return;
				}

				if (conditionUpdateTime > 0.0F && conditionUpdateTime < ActiveEffectConditionUpdateInterval()) {
					reinterpret_cast<float&>(activeEffect->pad94) += elapsedTimeDelta;
					return;
				}
			}

			if (activeEffect->effect->conditions.IsTrue(activeEffect->target->GetTargetStatsObject(), activeEffect->caster.get().get()) && !activeEffect->CheckDisplacementSpellOnTarget()) {
				activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kTrue;
				logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kTrue");
			}
			else {
				activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kFalse;
				logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kFalse");
			}
		}
		else {
			activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kNotAvailable;
			logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kNotAvailable");
		}
	}
}

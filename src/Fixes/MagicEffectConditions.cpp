#include "RE/Bethesda/Settings.hpp"
#include <RE/Bethesda/Actor.hpp>
#include "Internal/Config/Config.hpp"


namespace Internal::Fixes::MagicEffectConditions
{
	void Install() noexcept
	{
		if (!Config::bMagicEffectConditions.GetValue()) {
			logger::info("MagicEffectConditions -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}
		// should have the contents of Fix() probably
	}

	void Fix(bool& magicEffectConditions)
	{
		if (magicEffectConditions == true) {
			// temp to make the compiler chill
		}
		// Utility::Memory::SafeWriteAbsoluteJump(Addresses::Fixes::MagicEffectConditions::EvaluateConditions, reinterpret_cast<std::uintptr_t>(std::addressof(MagicEffectConditions::EvaluateConditions)));
	}

	// float MagicEffectConditions::ActiveEffectConditionUpdateInterval()
	// {
	// 	static auto* activeEffectConditionUpdateInterval = Skyrim::GameSettingCollection::InitializeSetting("fActiveEffectConditionUpdateInterval");

	// 	return activeEffectConditionUpdateInterval->GetFloat() > 0.0001F ? activeEffectConditionUpdateInterval->GetFloat() : 1.0F;
	// }
	float ActiveEffectConditionUpdateInterval()
	{
		static auto* conditionInterval = RE::GameSettingCollection::GetSingleton()->GetSetting("fActiveEffectConditionUpdateInterval");
		if (conditionInterval->GetFloat() > 0.001F) {
			return conditionInterval->GetFloat();
		} else {
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
			// TODO elapsedSeconds probably isnt correct
			auto& conditionUpdateTime = reinterpret_cast<float&>(activeEffect->elapsedSeconds);

			if (!forceUpdate) {
				if (activeEffect->elapsedSeconds <= 0.0F) {
					conditionUpdateTime = elapsedTimeDelta;
					return;
				}

				static const auto conditionInterval = ActiveEffectConditionUpdateInterval();
				if (conditionUpdateTime > 0.0F && conditionUpdateTime < ActiveEffectConditionUpdateInterval()) {
					conditionUpdateTime += elapsedTimeDelta;
					return;
				}
			}

			conditionUpdateTime = elapsedTimeDelta;

			auto target = activeEffect->caster.get();

			if (activeEffect->effect->conditions.IsTrue(activeEffect->target->GetTargetStatsObject(), target.get()) && !activeEffect->CheckDisplacementSpellOnTarget()) {
				activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kTrue;
				logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kTrue");
			} else {
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

// if (activeEffect->conditionStatus == ActiveEffect::ConditionStatus::kNotAvailable)
// return;

// if ((activeEffect->flags.all(ActiveEffect::Flags::kHasConditions) || activeEffect->unk70) && activeEffect->magicTarget && activeEffect->magicTarget->GetTargetStatsObject()) {
// if (!forceUpdate) {
// 	if ((*unkCheck & 1) == 0) {
// 		*unkCheck |= 1;
// 		float interval = fActiveEffectConditionUpdateInterval->GetFloat();
// 		if (interval != 0)
// 			*fcheckConditionsRate = 1.f / interval;
// 	}

// 	if (activeEffect->elapsed <= 0.0F) {
// 		reinterpret_cast<float&>(activeEffect->pad94) = deltaTime;
// 		return;
// 	}

// 	static const float interval = fActiveEffectConditionUpdateInterval->GetFloat();

// 	if (reinterpret_cast<float&>(activeEffect->pad94) > 0.0F && reinterpret_cast<float&>(activeEffect->pad94) < interval) {
// 		reinterpret_cast<float&>(activeEffect->pad94) += deltaTime;
// 		return;
// 	}
// }

// TESObjectREFR* target = activeEffect->target.get().get();
// if (activeEffect->effect->conditions.IsTrue(activeEffect->magicTarget->GetTargetStatsObject(), target) && !activeEffect->CheckDisplacementSpellOnTarget())
// 	activeEffect->conditionStatus = ActiveEffect::ConditionStatus::kTrue;
// else
// 	activeEffect->conditionStatus = ActiveEffect::ConditionStatus::kFalse;
// } else {
// activeEffect->conditionStatus = ActiveEffect::ConditionStatus::kNotAvailable;
// }
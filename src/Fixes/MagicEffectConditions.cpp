#include "RE/Bethesda/Settings.hpp"
#include <RE/Bethesda/Actor.hpp>

namespace Internal::Fixes::MagicEffectConditions
{
	void Install() noexcept
	{
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
		return conditionInterval->GetFloat() > 0.001F ? conditionInterval->GetFloat() : 1.0F;
	}

	void EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
	{
		if (activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			return;
		}

		if ((activeEffect->flags.all(RE::ActiveEffect::Flags::kHasConditions) || activeEffect->displacementSpell) && activeEffect->target && activeEffect->target->GetTargetStatsObject()) {
			float temp = 0.0;
			auto& conditionUpdateTime = temp;
			// auto& conditionUpdateTime = reinterpret_cast<float&>(activeEffect->padding8C); TODO

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

			auto* subject = activeEffect->target->GetTargetStatsObject();
			auto target = activeEffect->caster.get();

			activeEffect->conditionStatus = activeEffect->effect->conditions.IsTrue(subject, target.get()) && !activeEffect->CheckDisplacementSpellOnTarget() ? RE::ActiveEffect::ConditionStatus::kTrue : RE::ActiveEffect::ConditionStatus::kFalse;
		}
		else {
			activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kNotAvailable;
		}
	}
}
// void MagicEffectConditions::EvaluateConditions(Skyrim::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
// {

// 		conditionUpdateTime = elapsedTimeDelta;

// 		auto* subject = activeEffect->magicTarget->GetMagicTargetAsReference();
// 		auto  target  = activeEffect->casterHandle.get();

// 		activeEffect->conditionStatus = activeEffect->effectItem->conditions.IsTrue(subject, target.get()) && !activeEffect->CheckDisplacement() ?
// 		                                    Skyrim::ActiveEffect::ConditionStatus::kTrue :
// 		                                    Skyrim::ActiveEffect::ConditionStatus::kFalse;
// 	}
// 	else
// 	{
// 		activeEffect->conditionStatus = Skyrim::ActiveEffect::ConditionStatus::kNotAvailable;
// 	}
// }

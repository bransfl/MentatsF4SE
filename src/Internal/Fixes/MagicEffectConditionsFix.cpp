#include "Internal/Fixes/MagicEffectConditionsFix.hpp"
#include "Internal/Config.hpp"

namespace Internal::Fixes
{
	typedef void(EvaluateConditionsSig)(RE::ActiveEffect*, float, bool);
	REL::Relocation<EvaluateConditionsSig> OriginalFunction_EvaluateConditions;

	void MagicEffectConditionsFix::Install() noexcept
	{
		logger::info("Fix installing: MagicEffectConditionsFix."sv);

		if (!Config::bMagicEffectConditionsFix.GetValue()) {
			logger::info("Fix aborted: MagicEffectConditionsFix. Reason: Fix was disabled in config file."sv);
			return;
		}

		if (REX::W32::GetModuleHandleW(L"MGEFConditionFix.dll")) {
			logger::info("Fix aborted: MagicEffectConditionsFix. Reason: Mod was installed: MGEFConditionFix.dll."sv);
			return;
		}

		F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (REL::Module::IsNG()) {
			// NG Patch - TODO needs to be tested and fixed
			logger::info("Fix aborted: MagicEffectConditionsFix. Reason: Game version was Next-Gen."sv);
			return;
			// REL::Relocation<uintptr_t> ptr_EvaluateConditions_NG{ REL::ID(2226003) };
			// OriginalFunction_EvaluateConditions_NG = trampoline.write_branch<5>(ptr_EvaluateConditions_NG.address(), &Hook_EvaluateConditions);
		}
		else {
			// OG Patch
			REL::Relocation<uintptr_t> ptr_EvaluateConditions_OG{ REL::ID(1228998) };
			OriginalFunction_EvaluateConditions = trampoline.write_branch<5>(ptr_EvaluateConditions_OG.address(), &Hook_EvaluateConditions);
		}

		logger::info("Fix installed: MagicEffectConditionsFix."sv);
	}

	float MagicEffectConditionsFix::ActiveEffectConditionUpdateInterval()
	{
		RE::Setting* conditionUpdateInterval = RE::GameSettingCollection::GetSingleton()->GetSetting("fActiveEffectConditionUpdateInterval"sv);
		if (conditionUpdateInterval->GetFloat() > 0.001F) {
			return conditionUpdateInterval->GetFloat();
		}
		else {
			return 1.0F;
		}
	}

	// note - EffectSetting has a lot of magiceffect data
	// kNotAvailable = no conditions on the record

	// thank you bingle my beloved
	void MagicEffectConditionsFix::Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
	{
		// logger::info("activeEffect was: GetFormID()={:08X}, EditorId={}"sv, activeEffect->spell->GetFormID(), activeEffect->spell->GetFormEditorID());

		if (activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			// this effect has no conditions
			return;
		}

		if (activeEffect->spell->GetCastingType() == RE::MagicSystem::CastingType::kFireAndForget) {
			RE::AlchemyItem* potion = RE::TESForm::GetFormByID(activeEffect->spell->GetFormID())->As<RE::AlchemyItem>();
			if (potion) {
				if (potion->data.addictionChance > 0.0 || potion->data.addictionItem != nullptr) {
					// this is an addiction effect
					return;
				}
			}
		}

		// if (all effect conditions are true OR this is a displacement spell) AND (the target is valid AND the target's object is valid)
		if ((activeEffect->flags.all(RE::ActiveEffect::Flags::kHasConditions) || activeEffect->displacementSpell) && activeEffect->target && activeEffect->target->GetTargetStatsObject()) {

			// store the auxillary timer on the unused member variable pad94
			auto& conditionUpdateTime = reinterpret_cast<float&>(activeEffect->pad94);

			if (!forceUpdate) {
				if (activeEffect->elapsedSeconds <= 0.0F) {
					// set the auxillary timer to the amt of the effect has been active
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
			}
			else {
				activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kFalse;
			}
		}
		else {
			activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kNotAvailable;
		}
	}
}

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
			// OriginalFunction_EvaluateConditions = trampoline.write_branch<5>(ptr_EvaluateConditions_NG.address(), &Hook_EvaluateConditions);
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

	// thank you bingle my beloved
	void MagicEffectConditionsFix::Hook_EvaluateConditions(RE::ActiveEffect* a_activeEffect, float a_elapsedTimeDelta, bool a_forceUpdate)
	{
		// logger::debug("a_activeEffect was: GetFormID()={:08X}, EditorId={}"sv, a_activeEffect->spell->GetFormID(), a_activeEffect->spell->GetFormEditorID());

		if (a_activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			// this effect has no conditions, so there's nothing to evaluate
			return;
		}

		if (a_activeEffect->spell->GetCastingType() == RE::MagicSystem::CastingType::kFireAndForget) {
			RE::AlchemyItem* potion = RE::TESForm::GetFormByID(a_activeEffect->spell->GetFormID())->As<RE::AlchemyItem>();
			if (potion && potion->data.addictionChance > 0.0 || potion->data.addictionItem != nullptr) {
				// this is a fire-and-forget addictive potion (likely an addictive chem)
				// we return normally since fire-and-forget potions don't evaluate conditions after application
				return;
			}
		}

		// if (this effect has conditions OR this has a displacement spell) AND (the target is valid AND the target's object ref is valid)
		if ((a_activeEffect->flags.all(RE::ActiveEffect::Flags::kHasConditions) || a_activeEffect->displacementSpell) && a_activeEffect->target && a_activeEffect->target->GetTargetStatsObject()) {
			// store the auxillary timer on the unused uint32_t member pad94
			auto& conditionUpdateTime = reinterpret_cast<float&>(a_activeEffect->pad94);

			if (a_forceUpdate == false) {
				if (a_activeEffect->elapsedSeconds <= 0.0F) {
					// set the auxillary timer to the amt of time the effect has been active
					reinterpret_cast<float&>(a_activeEffect->pad94) = a_elapsedTimeDelta;
					return;
				}

				if (conditionUpdateTime > 0.0F && conditionUpdateTime < ActiveEffectConditionUpdateInterval()) {
					// add the effect's elapsed time to the auxillary timer
					reinterpret_cast<float&>(a_activeEffect->pad94) += a_elapsedTimeDelta;
					return;
				}
			}

			// evaluate conditions
			if (a_activeEffect->effect->conditions.IsTrue(a_activeEffect->target->GetTargetStatsObject(), a_activeEffect->caster.get().get()) && !a_activeEffect->CheckDisplacementSpellOnTarget()) {
				a_activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kTrue;
			}
			else {
				a_activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kFalse;
			}
		}
		else {
			a_activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kNotAvailable;
		}
	}
}

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

	// thank you bingle my beloved
	void MagicEffectConditionsFix::Hook_EvaluateConditions(RE::ActiveEffect* a_this, float a_elapsedTimeDelta, bool a_forceUpdate)
	{
		// logger::debug("a_this was: GetFormID()={:08X}, EditorId={}"sv, a_this->spell->GetFormID(), a_this->spell->GetFormEditorID());

		if (a_this->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			// this effect has no conditions, so there's nothing to evaluate
			return;
		}

		if (a_this->spell->GetCastingType() == RE::MagicSystem::CastingType::kFireAndForget) {
			RE::AlchemyItem* potion = RE::TESForm::GetFormByID(a_this->spell->GetFormID())->As<RE::AlchemyItem>();
			if (potion) {
				if (potion->data.addictionChance > 0.0 || potion->data.addictionItem != nullptr) {
					// this is a fire-and-forget addictive potion (likely an addictive chem)
					// we return normally since fire-and-forget potions don't evaluate conditions after application
					return;
				}
			}
		}

		// if (this effect has conditions OR this has a displacement spell) AND (the target is valid AND the target's object ref is valid)
		if ((a_this->flags.all(RE::ActiveEffect::Flags::kHasConditions) || a_this->displacementSpell) && a_this->target && a_this->target->GetTargetStatsObject()) {
			// store the auxillary timer on the unused uint32_t member pad94
			auto& conditionUpdateTime = reinterpret_cast<float&>(a_this->pad94);

			if (a_forceUpdate == false) {
				if (a_this->elapsedSeconds <= 0.0F) {
					// set the auxillary timer to the amt of time the effect has been active
					reinterpret_cast<float&>(a_this->pad94) = a_elapsedTimeDelta;
					return;
				}

				if (conditionUpdateTime > 0.0F && conditionUpdateTime < ActiveEffectConditionUpdateInterval()) {
					// add the effect's elapsed time to the auxillary timer
					reinterpret_cast<float&>(a_this->pad94) += a_elapsedTimeDelta;
					return;
				}
			}

			// evaluate conditions and set status accordingly since we replace the original function
			if (a_this->effect->conditions.IsTrue(a_this->target->GetTargetStatsObject(), a_this->caster.get().get()) && !a_this->CheckDisplacementSpellOnTarget()) {
				a_this->conditionStatus = RE::ActiveEffect::ConditionStatus::kTrue;
			}
			else {
				a_this->conditionStatus = RE::ActiveEffect::ConditionStatus::kFalse;
			}
		}
		else {
			a_this->conditionStatus = RE::ActiveEffect::ConditionStatus::kNotAvailable;
		}
	}
}

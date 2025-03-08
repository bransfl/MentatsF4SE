#include "Internal/Fixes/MagicEffectConditionsFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes
{
	typedef void(EvaluateConditionsSig)(RE::ActiveEffect*, float, bool);
	REL::Relocation<EvaluateConditionsSig> OriginalFunction_EvaluateConditions_OG;
	REL::Relocation<EvaluateConditionsSig> OriginalFunction_EvaluateConditions_NG;

	void MagicEffectConditionsFix::Install() noexcept
	{
		logger::info(FMT_STRING("Fix installing: MagicEffectConditionsFix."sv));

		if (!Config::bMagicEffectConditionsFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: MagicEffectConditionsFix. Reason: Fix was disabled in config file."sv));
			return;
		}
		if (REX::W32::GetModuleHandleW(L"MGEFConditionFix.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'MGEF Condition Fix' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info(FMT_STRING("Fix aborted: MagicEffectConditionsFix. Reason: Mod was installed: MGEFConditionFix.dll."sv));
			return;
		}

		F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (REL::Module::IsNG()) {
			// NG Patch
			REL::Relocation<uintptr_t> ptr_EvaluateConditions_NG{ REL::ID(2226003) };
			OriginalFunction_EvaluateConditions_NG = trampoline.write_branch<5>(ptr_EvaluateConditions_NG.address(), &Hook_EvaluateConditions);
		}
		else {
			// OG Patch
			REL::Relocation<uintptr_t> ptr_EvaluateConditions_OG{ REL::ID(1228998) };
			OriginalFunction_EvaluateConditions_OG = trampoline.write_branch<5>(ptr_EvaluateConditions_OG.address(), &Hook_EvaluateConditions);
		}

		logger::info(FMT_STRING("Fix installed: MagicEffectConditionsFix."sv));
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

	void MagicEffectConditionsFix::Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
	{
		if (activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			// logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was kNotAvailable. Return."sv);
			return;
		}

		// we check this to fix the addiction bug in the original mod
		if (activeEffect->spell->GetCastingType() == RE::MagicSystem::CastingType::kFireAndForget) {
			RE::TESForm* form = RE::TESForm::GetFormByID(activeEffect->spell->GetFormID());
			if (!form)
				return;

			RE::AlchemyItem* potion = form->As<RE::AlchemyItem>();
			if (!potion)
				return;

			// logger::debug(FMT_STRING("activeEffect was FaF potion. GetFormID()={:08X}, EditorId={}"), potion->GetFormID(), potion->GetFormEditorID());
			if (potion->data.addictionChance > 0.0 || potion->data.addictionItem != nullptr) {
				// logger::debug(FMT_STRING("activeEffect potion WAS addictive. addiction: {}"), potion->data.addictionName);
				return;
			}
		}

		// if (all effect conditions are true OR this is a displacement spell) AND (the target is valid AND the target's object is valid)
		if ((activeEffect->flags.all(RE::ActiveEffect::Flags::kHasConditions) || activeEffect->displacementSpell) && activeEffect->target && activeEffect->target->GetTargetStatsObject()) {

			// auxillary timer
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

			RE::TESObjectREFR* caster = activeEffect->caster.get().get(); // note - this might need to be target?
			if (activeEffect->effect->conditions.IsTrue(activeEffect->target->GetTargetStatsObject(), caster) && !activeEffect->CheckDisplacementSpellOnTarget()) {
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

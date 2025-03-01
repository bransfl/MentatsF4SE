#include "Internal/Fixes/MagicEffectConditionsFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::MagicEffectConditionsFix
{
	typedef void(EvaluateConditionsSig)(RE::ActiveEffect*, float, bool);
	REL::Relocation<EvaluateConditionsSig> OriginalFunction_EvaluateConditions;

	void Install() noexcept
	{
		logger::info("Fix installing: MagicEffectConditionsFix."sv);

		if (!Config::bMagicEffectConditionsFix.GetValue()) {
			logger::info("Fix aborted: MagicEffectConditionsFix. Reason: Fix was disabled in ini file."sv);
			return;
		}
		if (REX::W32::GetModuleHandleW(L"MGEFConditionFix.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'MGEF Condition Fix' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info("Fix aborted: MagicEffectConditionsFix. Reason: Mod was installed: MGEFConditionFix.dll."sv);
			return;
		}

		F4SE::AllocTrampoline(8 * 8);
		F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
		if (REL::Module::IsNG()) {
			logger::info("Fix aborted: MagicEffectConditionsFix. Reason: Game version was NG."sv);
			return;
			// NG Patch - Don't have address for this yet.
			// REL::Relocation<uintptr_t> ptr_EvaluateConditions_NG{ REL::ID() };
			// trampoline.write_branch<5>(ptr_EvaluateConditions_NG.address(), &EvaluateConditions); TODO
		}
		else {
			// OG Patch
			REL::Relocation<uintptr_t> evaluateConditionsFuncLocation{ REL::ID(1228998) };
			OriginalFunction_EvaluateConditions = trampoline.write_branch<5>(evaluateConditionsFuncLocation.address(), &Hook_EvaluateConditions);
		}

		logger::info("Fix installed: MagicEffectConditionsFix.");
	}

	float ActiveEffectConditionUpdateInterval()
	{
		static RE::Setting* conditionInterval = RE::GameSettingCollection::GetSingleton()->GetSetting("fActiveEffectConditionUpdateInterval"sv);
		if (conditionInterval->GetFloat() > 0.001F) {
			return conditionInterval->GetFloat();
		}
		else {
			return 1.0F;
		}
	}

	// the hook
	void Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
	{
		if (activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was kNotAvailable. Return."sv);
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

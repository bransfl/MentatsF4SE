#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::MagicEffectConditions
{
	typedef void(EvaluateConditionsSig)(RE::ActiveEffect*, float, bool);
	REL::Relocation<EvaluateConditionsSig> OriginalFunction_EvaluateConditions;

	void Install() noexcept
	{
		logger::info("Fix installing: MagicEffectConditions."sv);

		if (!Config::bMagicEffectConditions.GetValue()) {
			logger::info("Fix aborted: MagicEffectConditions. Reason: Fix was disabled in ini file."sv);
			return;
		}
		if (REX::W32::GetModuleHandleW(L"MGEFConditionFix.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'MGEF Condition Fix' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info("Fix aborted: MagicEffectConditions. Reason: Mod was installed: MGEFConditionFix.dll."sv);
			return;
		}

		F4SE::AllocTrampoline(8 * 8);
		F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
		if (Config::bIsNG == true) {
			logger::info("Fix aborted: MagicEffectConditions. Reason: Game version was NG."sv);
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

		logger::info("Fix installed: MagicEffectConditions.");
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
	// this is over-annotated because luca and i are confused
	void Hook_EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate)
	{
		// if conditions are missing or empty (?)
		if (activeEffect->conditionStatus == RE::ActiveEffect::ConditionStatus::kNotAvailable) {
			logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was kNotAvailable. Return."sv);
			return;
		}

		// note - if fire and forget (chems) call og func maybe?

		// if (all effect conditions are true OR this is a displacement spell) AND (the target is valid AND the target's object is valid)
		if ((activeEffect->flags.all(RE::ActiveEffect::Flags::kHasConditions) || activeEffect->displacementSpell) && activeEffect->target && activeEffect->target->GetTargetStatsObject()) {

			// store the auxillary timer on the unused member pad94
			auto& conditionUpdateTime = reinterpret_cast<float&>(activeEffect->pad94);

			if (!forceUpdate) {
				if (activeEffect->elapsedSeconds <= 0.0F) {
					// set the auxillary timer to the amt of the effect has been active
					reinterpret_cast<float&>(activeEffect->pad94) = elapsedTimeDelta;
					logger::debug("MagicEffectConditions -> activeEffect's pad94 was set to elapsedTimeDelta"sv);
					return;
				}

				if (conditionUpdateTime > 0.0F && conditionUpdateTime < ActiveEffectConditionUpdateInterval()) {
					reinterpret_cast<float&>(activeEffect->pad94) += elapsedTimeDelta;
					logger::debug("MagicEffectConditions -> activeEffect's pad94 had elapsedTimeDelta added."sv);
					return;
				}
			}

			if (activeEffect->effect->conditions.IsTrue(activeEffect->target->GetTargetStatsObject(), activeEffect->caster.get().get()) && !activeEffect->CheckDisplacementSpellOnTarget()) {
				activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kTrue;
				logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kTrue"sv);
			}
			else {
				activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kFalse;
				logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kFalse"sv);
			}
		}
		else {
			activeEffect->conditionStatus = RE::ActiveEffect::ConditionStatus::kNotAvailable;
			logger::debug("MagicEffectConditions -> activeEffect's ConditionStatus was set to kNotAvailable"sv);
		}
	}
}

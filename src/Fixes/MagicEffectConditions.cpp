#include "Internal/Fixes/MagicEffectConditions.hpp"
#include "Internal/Config/Config.hpp"
#include "RE/Bethesda/Settings.hpp"
#include <RE/Bethesda/Actor.hpp>

namespace Internal::Fixes::MagicEffectConditions
{
	// forward declaration for Install() since MagicEffectConditions.hpp is being annoying
	void EvaluateConditions(RE::ActiveEffect* activeEffect, float elapsedTimeDelta, bool forceUpdate);

	void Install() noexcept
	{
		if (!Config::bMagicEffectConditions.GetValue()) {
			logger::info("Fix aborted: bMagicEffectConditions. Reason: Fix was disabled in ini file.");
			return;
		}
		if (std::filesystem::exists("Data/F4SE/Plugins/MGEFConditionFix.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'MGEF Condition Fix' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info("Fix aborted: MagicEffectConditions. Reason: Mod was installed: MGEFConditionFix.dll.");
			return;
		}

		F4SE::AllocTrampoline(8 * 8);
		F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
		if (REL::Module::IsNG()) {
			logger::info("Fix aborted: bMagicEffectConditions. Reason: Game version was NG.");
			return;
			// NG Patch - Don't have address for this yet.
			// REL::Relocation<uintptr_t> ptr_EvaluateConditions_NG{ REL::ID() };
			// trampoline.write_branch<5>(ptr_EvaluateConditions_NG.address(), &EvaluateConditions); TODO
		}
		else {
			// OG Patch
			REL::Relocation<uintptr_t> ptr_EvaluateConditions_OG{ REL::ID(1228998) };
			trampoline.write_branch<5>(ptr_EvaluateConditions_OG.address(), &EvaluateConditions);
		}
		logger::info("Fix applied: OutfitRedressFix");
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
					logger::debug("MagicEffectConditions -> activeEffect's pad94 was set to elapsedTimeDelta");
					return;
				}

				if (conditionUpdateTime > 0.0F && conditionUpdateTime < ActiveEffectConditionUpdateInterval()) {
					reinterpret_cast<float&>(activeEffect->pad94) += elapsedTimeDelta;
					logger::debug("MagicEffectConditions -> activeEffect's pad94 had elapsedTimeDelta added.");
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

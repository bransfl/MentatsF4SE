#include "Internal/Fixes/PerkEntryPoints/ApplySpellsFix.hpp"
#include "Internal/Config/Config.hpp"

// fixes entrypoint ApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints
{
	void ApplySpellsFix::Install() noexcept
	{
		return;
		logger::info("Fix installing: ApplySpellsFix."sv);

		if (!Config::bApplySpellsFix.GetValue()) {
			logger::info("Fix aborted: ApplySpellsFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (REL::Module::IsNG()) {
			// NG Patch
		}
		else {
			// OG Patch
		}

		logger::info("Fix installed: ApplySpellsFix."sv);
	}

	// todo these likely dont work yet
	// static RE::BGSEntryPointPerkEntry* GetEntryPoint(RE::BGSEntryPoint::ENTRY_POINT entryPoint)
	// {
	// 	return std::addressof(GetEntryPoints()[std::to_underlying(entryPoint)]);
	// }

	// static RE::BGSEntryPointPerkEntry* GetEntryPoints()
	// {
	// 	// inline constexpr REL::RelocationID BGSEntryPointPerkEntry{ 928907, 2767025 };
	// 	if (REL::Module::IsNG()) {
	// 		REL::Relocation<uintptr_t> BGSEntryPointPerkEntry_Location{ REL::ID(2767025) };
	// 		auto* singleton{ reinterpret_cast<RE::BGSEntryPointPerkEntry*>(BGSEntryPointPerkEntry_Location.address()) };
	// 		return singleton;
	// 	}
	// 	else {
	// 		REL::Relocation<uintptr_t> BGSEntryPointPerkEntry_Location{ REL::ID(928907) };
	// 		auto* singleton{ reinterpret_cast<RE::BGSEntryPointPerkEntry*>(BGSEntryPointPerkEntry_Location.address()) };
	// 		return singleton;
	// 	}
	// }

	void ApplySpellsFix::ApplyCombatHitSpell(
		RE::BGSEntryPoint::ENTRY_POINT entryPoint,
		RE::Actor* perkOwner,
		...)
	{
		std::va_list varArgs;
		va_start(varArgs, perkOwner);
		auto* weapon = va_arg(varArgs, RE::TESObjectWEAP*);
		auto* target = va_arg(varArgs, RE::Actor*);
		// auto** result = va_arg(varArgs, RE::SpellItem**); unused?
		va_end(varArgs);

		// ApplySpells::HandleEntryPoint(entryPoint, perkOwner, std::vector<Skyrim::TESForm*>{ perkOwner, weapon, target });
		auto spellItemsList = ApplySpellsFix::HandleEntryPoint(entryPoint, perkOwner, std::vector<RE::TESForm*>{ perkOwner, weapon, target });

		for (auto* spellItem : spellItemsList) {
			if (spellItem) {
				// cast here
				// static_cast<Skyrim::SpellItem*>(combatHitSpellItem)->Apply(target, ApplySpells::castSpells_ ? perkOwner : target);
				// static_cast<RE::SpellItem*>(spellItem)->Cast(perkOwner, target, perkOwner, /* add vm here? */);
			}
		}
		// bool Cast(TESObjectREFR* caster, TESObjectREFR* target, Actor* anyActor, BSScript::IVirtualMachine* vm)
		// {
		// 	using func_t = decltype(&SpellItem::Cast);
		// 	static REL::Relocation<func_t> func{ REL::ID(1511987) };
		// 	return func(this, caster, target, anyActor, vm);
		// }
	}

	std::vector<void*> ApplySpellsFix::HandleEntryPoint(
		RE::BGSEntryPoint::ENTRY_POINT entryPoint,
		RE::Actor* perkOwner,
		std::vector<RE::TESForm*> conditionFilterArguments)
	{
		uint8_t entryPointUnderlying = static_cast<uint8_t>(std::to_underlying(entryPoint));

		// last enum type is (kSetFatigueToAPMult = 0x9D), so we can probably try 0x9E for skyrim's kTotal's equivalent

		if (entryPointUnderlying < 0x9D) {
			if (perkOwner && perkOwner->HasPerkEntries(entryPointUnderlying)) {

				// conditionFilterArgumentCount is likely either (std::uint8_t numArgs, or std::uint8_t unk03 (guessed due to uint8_t)
				// if (conditionFilterArguments.size() == GetEntryPoint(entryPoint)->entryData.numArgs) {
				// 	std::vector<void*> entryPointFunctionArgs = { std::numeric_limits<uint8_t>::max(), nullptr };
				// 	// RE::HandleEntryPointVisitor handlePerkEntryVisitor = RE::HandleEntryPointVisitor(
				// 	// 	perkOwner,
				// 	// 	(GetEntryPoint(entryPoint)->functionData->GetType()),
				// 	// 	(conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data()),
				// 	// 	(entryPointFunctionArgs.empty() ? nullptr : entryPointFunctionArgs.data()),
				// 	// 	static_cast<uint8_t>(conditionFilterArguments.size()),
				// 	// 	static_cast<uint8_t>(entryPointFunctionArgs.size()));

				// 	// perkOwner->ForEachPerkEntry(entryPoint, handleEntryPointVisitor);

				// 	std::erase(entryPointFunctionArgs, nullptr);

				// 	return entryPointFunctionArgs;
				// }
			}
		}

		return {};
	}
}

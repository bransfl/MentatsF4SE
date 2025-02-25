#include "Internal/Fixes/PerkEntryPoints/ApplySpellsFix.hpp"
#include "Internal/Config/Config.hpp"

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpellsFix
{
	// typedefs
	// arguments might be incorrect
	typedef RE::PerkEntryVisitor(HandlePerkEntryVisitorSig)(RE::BGSEntryPointPerkEntry* a_perkEntry);
	REL::Relocation<HandlePerkEntryVisitorSig> OriginalFunction_HandlePerkEntryVisitor;
	// inline constexpr REL::RelocationID __HandleEntryPointVisitor{ 721434, 2766957 };

	// Skyrim::HandleEntryPointVisitor handleEntryPointVisitor(
	// 	Skyrim::BGSEntryPoint::GetEntryPoint(entryPoint)->entryPointFunctionType.get(),
	// 	conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data(),
	// 	entryPointFunctionArguments.empty() ? nullptr : entryPointFunctionArguments.data(),
	// 	perkOwner,
	// 	static_cast<std::uint8_t>(conditionFilterArguments.size()),
	// 	static_cast<std::uint8_t>(entryPointFunctionArguments.size()));

	void Install() noexcept
	{
		logger::info("Fix installing: ApplySpellsFix."sv);

		if (!Config::bApplySpellsFix.GetValue()) {
			logger::info("Fix aborted: ApplySpellsFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (REL::Module::IsNG()) {
			// NG Patch
			logger::info("Fix aborted: ApplySpellsFix. Reason: Game version was NG."sv);
			return;
		}
		else {
			// OG Patch
			// REL::Relocation<uintptr_t> ptr_BGSEntryPoint_HandleEntryPoint_OG{ REL::ID(714336) };
			// trampoline.write_branch<5>(ptr_BGSEntryPoint_HandleEntryPoint_OG.address(), &ApplyCombatHitSpell);
		}

		logger::info("Fix installed: ApplySpellsFix."sv);
	}

	void ApplySpellsFix::ApplyCombatHitSpell(RE::BGSEntryPoint::ENTRY_POINT entryPoint,
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
			}
		}
	}

	// static BGSEntryPoint* GetEntryPoint(Utility::Enumeration<EntryPoint, std::uint32_t> entryPoint);

	// vanilla func
	// inline void HandleEntryPoint(ENTRY_POINT a_entryPoint, Actor* a_perkOwner, Args... a_args)
	// 	{
	// 		using func_t = decltype(&BGSEntryPoint::HandleEntryPoint<Args...>);
	// 		static REL::Relocation<func_t> func{ REL::RelocationID(714336, 2206243) };
	// 		return func(a_entryPoint, a_perkOwner, a_args...);
	// 	}

	// template <class... Args>
	// inline void HandleEntryPoint(ENTRY_POINT a_entryPoint, Actor* a_perkOwner, Args... a_args)

	std::vector<void*> ApplySpellsFix::HandleEntryPoint(
		RE::BGSEntryPoint::ENTRY_POINT entryPoint,
		RE::Actor* perkOwner,
		std::vector<RE::TESForm*> conditionFilterArguments)
	{
		// last enum type is (kSetFatigueToAPMult = 0x9D), so we can try 0x9E for kTotal's equivalent
		uint8_t entryPointValue = static_cast<uint8_t>(std::to_underlying(entryPoint));

		if (entryPointValue < 0x9D) {
			if (perkOwner && perkOwner->HasPerkEntries(entryPointValue)) {
				// conditionFilterArgumentCount is likely either (std::uint8_t numArgs, or std::uint8_t unk03;
				// if (conditionFilterArguments.size() == entryPoint.numArgs) {
				// 	std::vector<void*> entryPointFunctionArgs = { std::numeric_limits<std::uint8_t>::max(), nullptr };
				// }
			}
		}

		return {};
	}
}

// scrambled bugs code:

// std::vector<void*> ApplySpells::HandleEntryPoint(
// 	Utility::Enumeration<Skyrim::BGSEntryPoint::EntryPoint, std::uint32_t> entryPoint,
// 	Skyrim::Actor*                                                         perkOwner,
// 	std::vector<Skyrim::TESForm*>                                          conditionFilterArguments)
// {
// 	if (entryPoint.underlying() < Utility::Conversion::ToUnderlying(Skyrim::BGSEntryPoint::EntryPoint::kTotal))
// 	{
// 		if (perkOwner && perkOwner->HasPerkEntries(entryPoint.get()))
// 		{
// 			if (conditionFilterArguments.size() == Skyrim::BGSEntryPoint::GetEntryPoint(entryPoint)->conditionFilterArgumentCount)
// 			{
// 				std::vector<void*> entryPointFunctionArguments = { std::numeric_limits<std::uint8_t>::max(), nullptr };

// 				Skyrim::HandleEntryPointVisitor handleEntryPointVisitor(
// 					Skyrim::BGSEntryPoint::GetEntryPoint(entryPoint)->entryPointFunctionType.get(),
// 					conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data(),
// 					entryPointFunctionArguments.empty() ? nullptr : entryPointFunctionArguments.data(),
// 					perkOwner,
// 					static_cast<std::uint8_t>(conditionFilterArguments.size()),
// 					static_cast<std::uint8_t>(entryPointFunctionArguments.size()));

// 				perkOwner->ForEachPerkEntry(entryPoint.get(), handleEntryPointVisitor);

// 				std::erase(entryPointFunctionArguments, nullptr);

// 				return entryPointFunctionArguments;
// 			}
// 		}
// 	}

// 	return {};
// }

// /* Apply Combat Hit Spell
// 	* Perk Owner
// 	* Weapon
// 	* Target */
// void ApplySpells::ApplyCombatHitSpell(Utility::Enumeration<Skyrim::BGSEntryPoint::EntryPoint, std::uint32_t> entryPoint, Skyrim::Actor* perkOwner, ...)
// {
// 	std::va_list variadicArguments;
// 	va_start(variadicArguments, perkOwner);

// 	auto*  weapon = va_arg(variadicArguments, Skyrim::TESObjectWEAP*);
// 	auto*  target = va_arg(variadicArguments, Skyrim::Actor*);
// 	auto** result = va_arg(variadicArguments, Skyrim::SpellItem**);

// 	va_end(variadicArguments);

// 	auto combatHitSpellItems = ApplySpells::HandleEntryPoint(entryPoint, perkOwner, std::vector<Skyrim::TESForm*>{ perkOwner, weapon, target });

// 	for (auto* combatHitSpellItem : combatHitSpellItems)
// 	{
// 		if (combatHitSpellItem)
// 		{
// 			static_cast<Skyrim::SpellItem*>(combatHitSpellItem)->Apply(target, ApplySpells::castSpells_ ? perkOwner : target);
// 		}
// 	}
// }

#pragma once

#include "RE/Bethesda/PerkEntries.hpp"

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpells
{
	void Install() noexcept;

	class ApplySpells
	{
	public:
		static void ApplyCombatHitSpell(
			REX::Enum<RE::BGSPerkEntry::EntryPoint, std::uint8_t> entryPoint,
			RE::Actor* perkOwner,
			...);

		static std::vector<void*> HandleEntryPoint(
			REX::Enum<RE::BGSPerkEntry::EntryPoint, std::uint8_t> entryPoint,
			RE::Actor* perkOwner,
			std::vector<RE::TESForm*> conditionFilterArguments);
	};
}

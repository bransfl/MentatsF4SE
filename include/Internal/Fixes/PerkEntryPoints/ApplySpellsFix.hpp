#pragma once

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpellsFix
{
	void Install() noexcept;

	class ApplySpellsFix
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

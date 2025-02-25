#pragma once

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpellsFix
{
	void Install() noexcept;

	// RE::BGSEntryPointPerkEntry* GetEntryPoint(RE::BGSEntryPoint::ENTRY_POINT entryPoint);

	class ApplySpellsFix
	{
	public:
		static void ApplyCombatHitSpell(
			RE::BGSEntryPoint::ENTRY_POINT entryPoint,
			RE::Actor* perkOwner,
			...);

		static std::vector<void*> HandleEntryPoint(
			RE::BGSEntryPoint::ENTRY_POINT entryPoint,
			RE::Actor* perkOwner,
			std::vector<RE::TESForm*> conditionFilterArguments);
	};
}

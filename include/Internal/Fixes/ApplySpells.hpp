#pragma once

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::ApplySpells
{
	void Install() noexcept;

	// static void ApplyCombatHitSpell(
	// 	Utility::Enumeration<RE::BGSEntryPoint::EntryPoint, std::uint32_t> entryPoint,
	// 	Skyrim::Actor*                                                         perkOwner,
	// 	...);

	//void ApplyCombatHitSpell(RE::BGSEntryPoint entryPoint) noexcept;
}

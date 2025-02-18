#include "Internal/Config/Config.hpp"
#include "Internal/Fixes/PerkEntryPoints/ApplySpells.hpp"

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpells
{
	void Install() noexcept {
		if (!Config::bApplyCombatHitSpellFix.GetValue()) {
			logger::info("PerkEntryPoints::ApplySpells -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}
	}

	/* Apply Combat Hit Spell
	* Perk Owner
	* Weapon
	* Target */
// void ApplyCombatHitSpell(Utility::Enumeration<Skyrim::BGSEntryPoint::EntryPoint, std::uint32_t> entryPoint, Skyrim::Actor* perkOwner, ...)
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
}

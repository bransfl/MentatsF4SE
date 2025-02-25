#include "Internal/Fixes/WorkbenchSoundFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

// Sounds:
// UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
// UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
// UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

// Workbenches:
// workbenchWeaponsA "Weapons Workbench" [FURN:0017B3A4]
// workbenchWeaponsB "Weapons Workbench" [FURN:0017E787]
// WorkbenchPowerArmor "Power Armor Station" [FURN:00157FEB]
// WorkbenchPowerArmorSmall "Power Armor Station" [FURN:0013BD08]
// WorkbenchArmorA "Armor Workbench" [FURN:0012EA9B]
// WorkshopScavengingStation "Scavenging Station" [FURN:0008674C]

namespace Internal::Fixes::WorkbenchSoundFix
{
	static inline constexpr std::string_view FixCommand_SewingMachine = "ForceKillSound 1698201"sv;
	static inline constexpr std::string_view FixCommand_Weld = "ForceKillSound 2277843"sv;
	static inline constexpr std::string_view FixCommand_PressDrill = "ForceKillSound 1558738"sv;
	static inline constexpr std::string_view FixCommand_PressPower = "ForceKillSound 1558739"sv;

	void Install() noexcept
	{
		logger::info("Fix installing: WorkbenchSoundFix."sv);

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(Events::FurnitureEventHandler::GetSingleton());

		auto* cellHandler = Events::ActorCellEventHandler::GetSingleton();
		// pray for me bro
		RE::PlayerCharacter::GetSingleton()->RE::BSTEventSource<RE::BGSActorCellEvent>::RegisterSink(cellHandler);
		logger::info("WorkbenchSoundFix -> Events registered."sv);

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void KillSoundsAll()
	{
		Utility::Console::ExecuteCommand("ForceKillSound"sv, true);
	}

	void KillSoundSewingMachine()
	{
		Utility::Console::ExecuteCommand(FixCommand_SewingMachine, true);
	}

	void KillSoundWeld()
	{
		Utility::Console::ExecuteCommand(FixCommand_Weld, true);
	}

	void KillSoundPressDrill()
	{
		Utility::Console::ExecuteCommand(FixCommand_PressDrill, true);
	}

	void KillSoundPressPower()
	{
		Utility::Console::ExecuteCommand(FixCommand_PressPower, true);
	}

	std::vector<RE::TESObjectREFR*> GetFurnitureRefsInCell(RE::TESObjectCELL* a_cell)
	{
		auto refs = std::vector<RE::TESObjectREFR*>();
		if (a_cell == nullptr) {
			return refs;
		}

		a_cell->ForEachRef([&](RE::TESObjectREFR* a_ref) {
			if (a_ref->GetBaseObject()->formType == RE::ENUM_FORMTYPE::kFURN) {
				refs.push_back(a_ref);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return refs;
	}

	bool CheckWorkbench(RE::TESObjectREFR* furniture, bool killSound)
	{
		bool ret = false;
		switch (furniture->GetBaseObject()->formID) {
			case 0x17B3A4: { // workbenchWeaponsA
				ret = true;
				break;
			}
			case 0x17E787: { // workbenchWeaponsB
				ret = true;
				break;
			}
			case 0x157FEB: { // WorkbenchPowerArmor
				ret = true;
				break;
			}
			case 0x13BD08: { // WorkbenchPowerArmorSmall
				ret = true;
				break;
			}
			case 0x12EA9B: { // WorkbenchArmorA
				ret = true;
				break;
			}
			case 0x8674C: { // WorkshopScavengingStation
				ret = true;
				break;
			}
			default: {
				break;
			}
		}
		if (killSound && ret) {
			KillSoundsAll();
		}
		return ret;
	}

	namespace Events
	{
		//
	}
}

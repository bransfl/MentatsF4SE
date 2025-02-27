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
	void Install() noexcept
	{
		logger::info("Fix installing: WorkbenchSoundFix."sv);

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// furniture
		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(Events::FurnitureEventHandler::GetSingleton());
		// cells
		auto* cellHandler = Events::ActorCellEventHandler::GetSingleton();
		RE::PlayerCharacter::GetSingleton()->RE::BSTEventSource<RE::BGSActorCellEvent>::RegisterSink(cellHandler);
		logger::info("WorkbenchSoundFix -> Events registered."sv);

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void FixWorkbenchSound(RE::TESFurniture* a_workbench)
	{
		if (a_workbench == nullptr) {
			logger::info("WorkbenchSoundFix -> a_workbench was nullptr"sv);
			return;
		}
		// this might need to be passed as TESObjectREFR to recieve anim graph events, need to test

		// send furnitureExitEvent animation event to a_workbench to kill animations/sound
		// todo - try sending the event to the player, and try sending it with StopSound SoundEditorID
		// a_workbench->NotifyAnimationGraphImpl("furnitureExitSlave");
		logger::info("WorkbenchSoundFix -> FixWorkbenchSound ran"sv);
	}

	std::vector<RE::TESObjectREFR*> GetFurnitureRefsInCell(RE::TESObjectCELL* a_cell)
	{
		auto refs = std::vector<RE::TESObjectREFR*>();

		if (a_cell == nullptr) {
			return refs;
		}

		a_cell->ForEachRef([&](RE::TESObjectREFR* a_ref) {
			if (a_ref && (a_ref->GetBaseObject()->formType == RE::ENUM_FORMTYPE::kFURN)) {
				refs.push_back(a_ref);
			}
			return RE::BSContainer::ForEachResult::kContinue;
		});

		return refs;
	}

	// checks if the given furniture is a valid workbench
	bool IsWorkbench(RE::TESFurniture* a_furniture)
	{
		if (!a_furniture) {
			return false;
		}

		return a_furniture->wbData.type != RE::WorkbenchData::Type::kNone;
	}

	namespace Events
	{
		//
	}
}

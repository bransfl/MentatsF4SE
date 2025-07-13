#include "Internal/Fixes/WorkbenchSoundFix.hpp"

// Workshop UI SoundDescriptors:
// 	UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// 	UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
//	UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
//	UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

namespace Internal::Fixes
{
	// map: furniture formid : console command
	static inline const std::unordered_map<RE::TESFormID, std::string_view> furnitureCommandMap = {
		{ 0x08674C, "RecvAnimEvent \"SoundStop\" \"NPCHumanWeldLPM\""sv },																			 // WorkshopScavengingStation
		{ 0x12EA9B, "RecvAnimEvent \"SoundStop\" \"UIWorkshopSewingMachineRunLPM\""sv },															 // WorkbenchArmorA
		{ 0x157FEB, "RecvAnimEvent \"SoundStop\" \"UIWorkshopPowerArmorWeldLPM\""sv },																 // WorkbenchPowerArmor
		{ 0x13BD08, "RecvAnimEvent \"SoundStop\" \"UIWorkshopPowerArmorWeldLPM\""sv },																 // WorkbenchPowerArmorSmall
		{ 0x17B3A4, "RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressDrillLPM\";RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressPowerLPM\""sv }, // workbenchWeaponsA
		{ 0x17E787, "RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressDrillLPM\";RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressPowerLPM\""sv }, // workbenchWeaponsB
	};

	void WorkbenchSoundFix::Install() noexcept
	{
		logger::info("Fix installing: WorkbenchSoundFix."sv);

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in config."sv);
			return;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(WorkbenchSoundFix::FurnitureEventHandler::GetSingleton());
		player->RE::BSTEventSource<RE::BGSActorCellEvent>::RegisterSink(WorkbenchSoundFix::ActorCellEventHandler::GetSingleton());

		FixWorkbenchSounds(player, nullptr); // for existing saves and sanity

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void WorkbenchSoundFix::FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser, RE::TESFurniture* a_furniture) noexcept
	{
		if (!a_workbenchUser) {
			return;
		}

		if (a_furniture) {
			// process the furniture
			RE::TESFormID furnitureFormID = a_furniture->GetFormID();
			for (const auto& elem : furnitureCommandMap) {
				if (elem.first == furnitureFormID) {
					Utility::ExecuteCommand(elem.second, a_workbenchUser, true);
				}
			}
		}
		else {
			// process all commands on the user
			for (const auto& elem : furnitureCommandMap) {
				Utility::ExecuteCommand(elem.second, a_workbenchUser, true);
			}
		}
	}

	// bool WorkbenchSoundFix::IsWorkbench(RE::TESFurniture* a_furniture) noexcept
	// {
	// 	if (!a_furniture) {
	// 		return false;
	// 	}
	// 	return a_furniture->wbData.type != RE::WorkbenchData::Type::kNone;
	// }

	RE::BSEventNotifyControl WorkbenchSoundFix::FurnitureEventHandler::ProcessEvent(const RE::TESFurnitureEvent& a_event, RE::BSTEventSource<RE::TESFurnitureEvent>*)
	{
		if (a_event.IsEnter()) {
			// we only run on IsExit(), otherwise all workbenches would be silent
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::TESFurniture* furniture = a_event.targetFurniture.get()->As<RE::TESFurniture>();
		if (!furniture) {
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::TESObjectREFR* actor = a_event.actor.get();
		if (!actor) {
			return RE::BSEventNotifyControl::kContinue;
		}

		FixWorkbenchSounds(actor, furniture);

		return RE::BSEventNotifyControl::kContinue;
	}
}

#include "Internal/Fixes/WorkbenchSoundFix.hpp"

// Workshop UI SoundDescriptors:
// 	UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// 	UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
//	UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
//	UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

namespace Internal::Fixes
{
	// furniture formid : console command
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
		if (!player) {
			return;
		}
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
			// silence the sound annotation for the given furniture
			RE::TESFormID furnitureFormID = a_furniture->GetFormID();
			if (furnitureCommandMap.contains(furnitureFormID)) {
				Utility::ExecuteCommand(furnitureCommandMap.at(furnitureFormID), nullptr, true);
			}
		}
		else {
			// silence all sound annotations
			for (const auto& elem : furnitureCommandMap) {
				Utility::ExecuteCommand(elem.second, a_workbenchUser, true);
			}
		}
	}

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

	RE::BSEventNotifyControl WorkbenchSoundFix::ActorCellEventHandler::ProcessEvent(const RE::BGSActorCellEvent& a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*)
	{
		RE::TESObjectREFR* actor = a_event.actor.get().get();
		if (!actor) {
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::UI* ui = RE::UI::GetSingleton();
		if (!ui) {
			return RE::BSEventNotifyControl::kContinue;
		}
		if (ui->GetMenuOpen("ExamineMenu"sv) || ui->GetMenuOpen("CookingMenu"sv)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		FixWorkbenchSounds(actor, nullptr);

		return RE::BSEventNotifyControl::kContinue;
	}
}

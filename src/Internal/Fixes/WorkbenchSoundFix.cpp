#include "Internal/Fixes/WorkbenchSoundFix.hpp"

// Workshop UI SoundDescriptors:
// 	UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// 	UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
//	UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
//	UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

namespace Internal::Fixes
{
	static inline constexpr std::array<std::string_view, 4> FixWorkbenchSoundCommands = {
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopSewingMachineRunLPM\""sv,
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopPowerArmorWeldLPM\""sv,
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressDrillLPM\""sv,
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressPowerLPM\""sv,
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

		FixWorkbenchSounds(player); // for existing saves and sanity

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void WorkbenchSoundFix::FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser) noexcept
	{
		logger::info("WorkbenchSoundFix -> FixWorkbenchSounds ran."sv);

		for (const std::string_view& command : FixWorkbenchSoundCommands) {
			Utility::ExecuteCommand(command, a_workbenchUser, true);
		}
	}

	bool WorkbenchSoundFix::IsWorkbench(RE::TESFurniture* a_furniture) noexcept
	{
		if (!a_furniture) {
			return false;
		}

		return a_furniture->wbData.type != RE::WorkbenchData::Type::kNone;
	}

	RE::BSEventNotifyControl WorkbenchSoundFix::FurnitureEventHandler::ProcessEvent(const RE::TESFurnitureEvent& a_event, RE::BSTEventSource<RE::TESFurnitureEvent>*)
	{
		if (a_event.IsEnter()) {
			// we only run on IsExit(), otherwise workbenches would be silent
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::TESFurniture* a_furniture = a_event.targetFurniture.get()->As<RE::TESFurniture>();
		if (!a_furniture || !IsWorkbench(a_furniture)) {
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::TESObjectREFR* actor = a_event.actor.get();
		if (!actor) {
			return RE::BSEventNotifyControl::kContinue;
		}

		FixWorkbenchSounds(actor);

		return RE::BSEventNotifyControl::kContinue;
	}
}

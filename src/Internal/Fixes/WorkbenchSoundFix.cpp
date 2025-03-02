#include "Internal/Fixes/WorkbenchSoundFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

// SoundDescriptors:
// UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
// UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
// UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

namespace Internal::Fixes
{
	static inline constexpr std::array FixWorkbenchSoundCommands = {
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopSewingMachineRunLPM\""sv,
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopPowerArmorWeldLPM\""sv,
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressDrillLPM\""sv,
		"RecvAnimEvent \"SoundStop\" \"UIWorkshopDrillPressPowerLPM\""sv,
	};

	// RE::TESFaction* followerFaction = nullptr;	// todo - unused until i can figure out how to get the player's companions

	void WorkbenchSoundFix::Install() noexcept
	{
		logger::info("Fix installing: WorkbenchSoundFix."sv);

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(WorkbenchSoundFix::FurnitureEventHandler::GetSingleton());

		auto* cellHandler = WorkbenchSoundFix::ActorCellEventHandler::GetSingleton();
		RE::PlayerCharacter::GetSingleton()->RE::BSTEventSource<RE::BGSActorCellEvent>::RegisterSink(cellHandler);
		logger::info("WorkbenchSoundFix -> Events registered."sv);

		// followerFaction = (RE::TESFaction*)RE::TESForm::GetFormByID(0x23C01);

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void WorkbenchSoundFix::FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser)
	{
		logger::info("WorkbenchSoundFix -> FixWorkbenchSounds ran"sv);

		for (const auto& command : FixWorkbenchSoundCommands) {
			Utility::ExecuteCommand(command, a_workbenchUser, true);
		}
	}

	bool WorkbenchSoundFix::IsWorkbench(RE::TESFurniture* a_furniture)
	{
		if (!a_furniture) {
			return false;
		}

		return a_furniture->wbData.type != RE::WorkbenchData::Type::kNone;
	}

	bool WorkbenchSoundFix::IsPlayerCompanion(RE::Actor* a_actor)
	{
		if (!a_actor) {
			return false;
		}
		return false; // TODO
	}
}

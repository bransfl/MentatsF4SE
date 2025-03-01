#include "Internal/Fixes/WorkbenchSoundFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

// SoundDescriptors:
// UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
// UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
// UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

namespace Internal::Fixes::WorkbenchSoundFix
{
	std::string_view fixCommandArmorWorkbench = "RecvAnimEvent \"SoundStop\" \"UIWorkshopSewingMachineRunLPM\"";

	RE::TESFaction* followerFaction = nullptr;

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

		followerFaction = (RE::TESFaction*)RE::TESForm::GetFormByID(0x23C01);

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void FixWorkbenchSound()
	{
		Utility::Console::ExecuteCommand(fixCommandArmorWorkbench, RE::PlayerCharacter::GetSingleton(), true);

		logger::info("WorkbenchSoundFix -> FixWorkbenchSound ran"sv);
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

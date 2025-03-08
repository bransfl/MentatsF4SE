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

	void WorkbenchSoundFix::Install() noexcept
	{
		logger::info(FMT_STRING("Fix installing: WorkbenchSoundFix."sv));

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in config file."sv));
			return;
		}

		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(WorkbenchSoundFix::FurnitureEventHandler::GetSingleton());

		RE::PlayerCharacter::GetSingleton()->RE::BSTEventSource<RE::BGSActorCellEvent>::RegisterSink(WorkbenchSoundFix::ActorCellEventHandler::GetSingleton());
		logger::info(FMT_STRING("WorkbenchSoundFix -> Events registered."sv));

		logger::info(FMT_STRING("Fix installed: WorkbenchSoundFix."sv));
	}

	void WorkbenchSoundFix::FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser)
	{
		logger::info(FMT_STRING("WorkbenchSoundFix -> FixWorkbenchSounds ran"sv));

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
}

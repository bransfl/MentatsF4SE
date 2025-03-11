#include "Internal/Fixes/WorkbenchSoundFix.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility.hpp"

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
		logger::info("Fix installing: WorkbenchSoundFix."sv);

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in config file."sv);
			return;
		}

		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(WorkbenchSoundFix::FurnitureEventHandler::GetSingleton());
		RE::PlayerCharacter::GetSingleton()->RE::BSTEventSource<RE::BGSActorCellEvent>::RegisterSink(WorkbenchSoundFix::ActorCellEventHandler::GetSingleton());

		FixWorkbenchSounds(RE::PlayerCharacter::GetSingleton()); // for existing saves + sanity

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void WorkbenchSoundFix::FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser)
	{
		logger::info("WorkbenchSoundFix -> FixWorkbenchSounds ran."sv);

		for (const std::string_view& command : FixWorkbenchSoundCommands) {
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

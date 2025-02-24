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
	static inline constexpr std::string_view FixCommand_SewingMachine = "ForceKillSound 0019E999"sv;
	static inline constexpr std::string_view FixCommand_Weld = "ForceKillSound 0022C1D3"sv;
	static inline constexpr std::string_view FixCommand_PressDrill = "ForceKillSound 0017C8D2"sv;
	static inline constexpr std::string_view FixCommand_PressPower = "ForceKillSound 0017C8D3"sv;

	void Install() noexcept
	{
		logger::info("Fix installing: WorkbenchSoundFix."sv);

		if (!Config::bWorkbenchSoundFix.GetValue()) {
			logger::info("Fix aborted: WorkbenchSoundFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// register cellchange/load/furniture exit/etc listeners

		logger::info("Fix installed: WorkbenchSoundFix."sv);
	}

	void KillSounds()
	{
		RE::ConsoleLog::GetSingleton()->AddString("WorkbenchSoundFix KillSounds ran\n");
		Utility::Console::ExecuteCommand(FixCommand_SewingMachine, true);
		Utility::Console::ExecuteCommand(FixCommand_Weld, true);
		Utility::Console::ExecuteCommand(FixCommand_PressDrill, true);
		Utility::Console::ExecuteCommand(FixCommand_PressPower, true);
	}

	namespace Events
	{
		namespace Cell
		{

		}

		namespace Furniture
		{

		}
	}
}

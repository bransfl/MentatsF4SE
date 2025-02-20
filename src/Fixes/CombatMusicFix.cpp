#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
#include "RE/Bethesda/Events.hpp"

// prevents combat music from playing when not intended to play
namespace Internal::Fixes::CombatMusicFix
{

	void Install() noexcept
	{
		logger::info("Fix installing: CombatMusicFix.");

		RE::TESDeathEvent::GetEventSource()->RegisterSink(Events::DeathEventHandler::GetSingleton());

		logger::info("Fix installed: CombatMusicFix.");
	}

	// console cmds to run to stop all combat music
	static inline const std::vector<std::string> StopCombatMusic = {
		"RemoveMusic MUSzCombat",
		"RemoveMusic MUSzCombatBoss",
		"RemoveMusic MUSzCombatBossLegendary",
		"RemoveMusic MUSzCombatHigh",
		"RemoveMusic MUSzCombatInst307",
		"RemoveMusic MUSzCombatInst307Boss",
		"RemoveMusic MUSzCombatMassFusion",
		"RemoveMusic MUSzDLC01CombatMechanist",
		"RemoveMusic MUSzDLC01CombatMechanistMinions",
	};

	// please (just) stop the music!
	void Fix()
	{
		logger::info("CombatMusicFix -> Fix ran.");
		for (const auto& command : StopCombatMusic) {
			Utility::Console::ExecuteCommand(command, false);
		}
	}

	bool CheckNeedsFix()
	{
		bool ret = false;	// setup this way for logging
		auto playerCharacter = RE::PlayerCharacter::GetSingleton();
		if (playerCharacter && !playerCharacter->IsInCombat()) {
			ret = true;
		}
		logger::info("CombatMusicFix -> CheckNeedsFix() -> ret={}", ret);
		return ret;
	}
}

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

		logger::info("Fix installed: CombatMusicFix.");
	}

	void InitFix()
	{
		auto* eventSink = EventSink::GetSingleton();
		RE::TESDeathEvent::GetEventSource()->RegisterSink(eventSink);
	}

	// console cmds to run to stop music
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
		logger::info("CombatMusicFix - fix ran");
		for (const auto& command : StopCombatMusic) {
			Utility::Console::ExecuteCommand(command, false);
		}
	}
}

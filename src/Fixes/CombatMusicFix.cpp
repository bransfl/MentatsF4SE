#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

// prevents combat music from playing when not intended to play
namespace Internal::Fixes::CombatMusicFix
{

	void Install() noexcept
	{
		logger::info("Fix installing: CombatMusicFix."sv);

		if(!Config::bCombatMusicFix.GetValue()) {
			logger::info("Fix aborted: CombatMusicFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		RE::TESDeathEvent::GetEventSource()->RegisterSink(Events::DeathEventHandler::GetSingleton());

		logger::info("Fix installed: CombatMusicFix."sv);
	}

	// console cmds to run to stop all combat music
	static inline constexpr std::array StopCombatMusic = {
		"RemoveMusic MUSzCombat"sv,
		"RemoveMusic MUSzCombatBoss"sv,
		"RemoveMusic MUSzCombatBossLegendary"sv,
		"RemoveMusic MUSzCombatHigh"sv,
		"RemoveMusic MUSzCombatInst307"sv,
		"RemoveMusic MUSzCombatInst307Boss"sv,
		"RemoveMusic MUSzCombatMassFusion"sv,
		"RemoveMusic MUSzDLC01CombatMechanist"sv,
		"RemoveMusic MUSzDLC01CombatMechanistMinions"sv,
	};

	// please (just) stop the music!
	void Fix()
	{
		logger::info("CombatMusicFix -> Fix ran.");
		// todo - make this asynchronous and add a 3-5 second delay so it doesnt instantly end
		for (const auto& command : StopCombatMusic) {
			Utility::Console::ExecuteCommand(command, true);
		}
	}

	bool CheckNeedsFix()
	{
		bool ret = false;	// setup this way for logging
		auto playerCharacter = RE::PlayerCharacter::GetSingleton();
		if (playerCharacter && !playerCharacter->IsInCombat()) {
			ret = true;
		}
		logger::info("CombatMusicFix -> CheckNeedsFix() -> ret={}"sv, ret);
		return ret;
	}
}

#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
#include "RE/Bethesda/Events.hpp"

// prevents combat music from playing when not intended to play
namespace Internal::Fixes::CombatMusicFix
{

	void Install() noexcept
	{
		logger::info("Fix installing: CombatMusicFix."sv);

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
// static void fix()
// {
// 	auto asyncFunc = []() {
// 		std::this_thread::sleep_for(std::chrono::seconds(5));
// 		for (const auto& command : stopCombatMusic) {
// 			inlineUtils::sendConsoleCommand(command);
// 		}
// 	};
// 	std::jthread t(asyncFunc);
// 	t.detach();
// }
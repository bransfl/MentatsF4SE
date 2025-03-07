#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

// prevents combat music from playing when not intended to play
namespace Internal::Fixes
{
	static inline constexpr std::array StopCombatMusicCommands = {
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

	void CombatMusicFix::Install() noexcept
	{
		logger::info(FMT_STRING("Fix installing: CombatMusicFix."sv));

		if (!Config::bCombatMusicFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: CombatMusicFix. Reason: Fix was disabled in config file."sv));
			return;
		}

		RE::TESDeathEvent::GetEventSource()->RegisterSink(CombatMusicFix::DeathEventHandler::GetSingleton());

		logger::info(FMT_STRING("Fix installed: CombatMusicFix."sv));
	}

	// please (just) stop the music!
	void CombatMusicFix::Fix()
	{
		// asynchronous delay instead of just sleeping for 5 seconds
		auto asyncFunc = []() {
			std::this_thread::sleep_for(std::chrono::seconds(5));
			for (const auto& command : StopCombatMusicCommands) {
				Utility::ExecuteCommand(command, nullptr, true);
			}
		};
		std::jthread t(asyncFunc);
		t.detach();
	}

	bool CombatMusicFix::NeedsFix()
	{
		auto playerCharacter = RE::PlayerCharacter::GetSingleton();
		if (playerCharacter && !playerCharacter->IsInCombat()) {
			return true;
		}
		else {
			return false;
		}
	}
}

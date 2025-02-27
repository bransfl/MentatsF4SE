#include "Internal/Fixes/RaceChangeFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::RaceChangeFix
{
	void Install() noexcept
	{
		logger::info("RaceChangeFix -> Fix installing..."sv);

		if (!Config::bRaceChangeFix.GetValue()) {
			logger::info("Fix aborted: RaceChangeFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// check for existing fix mods
		if (REL::Module::IsNG()) {
			// Race Changer NG
			if (REX::W32::GetModuleHandleW(L"f4se-race-changer.dll")) {
				RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Fix aborted: RaceChangeFix. Reason: Mod 'Race Changer NG' was detected. This fix is not necessary with this mod installed.\n");
				logger::warn("Fix aborted: RaceChangeFix. Reason: Mod was installed: f4se-race-changer.dll."sv);
				return;
			}
		}
		else {
			// Race Changer
			if (REX::W32::GetModuleHandleW(L"RaceChanger.dll")) {
				RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Fix aborted: RaceChangeFix. Reason: Mod 'Race Changer' was detected. This fix is not necessary with this mod installed.\n");
				logger::warn("Fix aborted: RaceChangeFix. Reason: Mod was installed: RaceChanger.dll."sv);
				return;
			}
		}

		Fix();

		logger::info("RaceChangeFix -> Fix installed."sv);
	}

	void Fix()
	{
		logger::info("RaceChangeFix -> Fix running..."sv);

		const auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			logger::info("Fix aborted: RaceChangeFix. Reason: Player was null."sv);
			return;
		}
		
		if (player->charGenRace != player->race) {
			player->charGenRace = player->race;
		}

		logger::info("RaceChangeFix -> Fix ran."sv);
	}
}

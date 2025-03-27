#include "Internal/Fixes/CombatMusicFix.hpp"

// prevents combat music from playing when not intended to play
namespace Internal::Fixes
{
	static inline constexpr std::array commands = {
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
		logger::info("Fix installing: CombatMusicFix."sv);

		if (!Config::bCombatMusicFix.GetValue()) {
			logger::info("Fix aborted: CombatMusicFix. Reason: Fix was disabled in config."sv);
			return;
		}

		RE::TESDeathEvent::GetEventSource()->RegisterSink(CombatMusicFix::DeathEventHandler::GetSingleton());

		logger::info("Fix installed: CombatMusicFix."sv);
	}

	// please (just) stop the music!
	void CombatMusicFix::Fix()
	{
		auto asyncFunc = []() {
			std::this_thread::sleep_for(std::chrono::seconds(5));
			for (const std::string_view& command : commands) {
				Utility::ExecuteCommand(command, nullptr, true);
			}
		};
		std::jthread t(asyncFunc);
		t.detach();
	}

	bool CombatMusicFix::NeedsFix() noexcept
	{
		auto playerCharacter = RE::PlayerCharacter::GetSingleton();
		if (playerCharacter && !playerCharacter->IsInCombat()) {
			return true;
		}
		else {
			return false;
		}
	}

	RE::BSEventNotifyControl CombatMusicFix::DeathEventHandler::ProcessEvent(const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		if (a_event.dying == true) {
			// the actor hasnt finished dying,escape early
			return RE::BSEventNotifyControl::kContinue;
		}

		if (a_event.actorDying.get() == nullptr || a_event.actorKiller.get() == nullptr) {
			// one of the actors was nullptr, escape early
			return RE::BSEventNotifyControl::kContinue;
		}

		if (a_event.actorKiller.get() != RE::PlayerCharacter::GetSingleton()) {
			// we dont need to run this for every single kill
			return RE::BSEventNotifyControl::kContinue;
		}

		if (NeedsFix()) {
			Fix();
		}

		return RE::BSEventNotifyControl::kContinue;
	}
}

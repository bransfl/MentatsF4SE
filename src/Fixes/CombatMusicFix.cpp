#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
#include "RE/Bethesda/Events.hpp"

#pragma warning(disable : 4100)

// prevents combat music from playing when not intended to play
namespace Internal::Fixes::CombatMusicFix
{

	void Install() noexcept
	{
		logger::info("Fix installing: CombatMusicFix.");

		TESDeathEventListener* deathEvent = new TESDeathEventListener();
		RE::TESDeathEvent::GetEventSource()->RegisterSink(deathEvent);

		logger::info("Fix installed: CombatMusicFix.");
	}

	// console cmds to run to stop music
	static inline const std::vector<std::string> StopCombatMusic = {
		"removemusic MUScombat",
	};
	// todo - this needs the other music types, and to be silent. sorry vish

	// please (just) stop the music!
	void Fix()
	{
		for (const auto& command : StopCombatMusic) {
			Utility::Console::ExecuteCommand(command);
		}
	}

	TESDeathEventListener* TESDeathEventListener::GetSingleton()
	{
		static TESDeathEventListener instance;
		return &instance;
	}

	RE::BSEventNotifyControl Internal::Fixes::CombatMusicFix::TESDeathEventListener::ProcessEvent(const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		auto playerCharacter = RE::PlayerCharacter::GetSingleton();
		if (playerCharacter && !playerCharacter->IsInCombat()) {
			Internal::Fixes::CombatMusicFix::Fix();
			logger::info("CombatMusicFix -> Fix ran through TESDeathEventListener event.");
		}
		return RE::BSEventNotifyControl::kContinue;
	}
}

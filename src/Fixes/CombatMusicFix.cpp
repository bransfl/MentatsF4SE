#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
#include "RE/Bethesda/Events.hpp"

#pragma warning(disable : 4100)

namespace Internal::Fixes::CombatMusicFix
{
	// console commands to stop the music
	static inline const std::vector<std::string> stopCombatMusic = {
		"removemusic MUScombat",
	};

	void Install() noexcept
	{
		TESDeathEventListener* deathEvent = new TESDeathEventListener();
		RE::TESDeathEvent::GetEventSource()->RegisterSink(deathEvent);
	}

	void Fix()
	{
		for (const auto& command : stopCombatMusic) {
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

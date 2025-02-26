#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/RaceChangeFix.hpp"
#include "Internal/Fixes/WorkbenchSoundFix.hpp"

namespace Internal::Messaging
{
	// handles various F4SE callback events
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		logger::info("Messaging: Received message of type: {}"sv, a_msg->type);

		switch (a_msg->type) {
			case F4SE::MessagingInterface::kPostLoad: {
				break;
			}
			case F4SE::MessagingInterface::kPostPostLoad: {
				break;
			}
			case F4SE::MessagingInterface::kPreLoadGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostLoadGame: {
				// CombatMusicFix
				if (Internal::Fixes::CombatMusicFix::NeedsFix()) {
					Internal::Fixes::CombatMusicFix::Fix();
				}

				// RaceChangeFix
				Internal::Fixes::RaceChangeFix::Install();

				// WorkbenchSoundFix
				// Internal::Fixes::WorkbenchSoundFix::Install();

				break;
			}
			case F4SE::MessagingInterface::kPreSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kDeleteGame: {
				break;
			}
			case F4SE::MessagingInterface::kInputLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kNewGame: {
				break;
			}
			case F4SE::MessagingInterface::kGameLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kGameDataReady: {
				// CombatMusicFix
				Internal::Fixes::CombatMusicFix::Install();

				// LeveledListCrashFix
				Internal::Fixes::LeveledListCrashFix::Sanitize();

				break;
			}
			default: {
				break;
			}
		}

		logger::info("Messaging: Finished processing for message of type: {}"sv, a_msg->type);
	}
}

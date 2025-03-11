#include "Internal/Messaging.hpp"
// fixes
#include "Internal/Fixes/ActorCauseSaveBloatFix.hpp"
#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/StolenPowerArmorOwnershipFix.hpp"
#include "Internal/Fixes/WorkbenchSoundFix.hpp"
// patches
#include "Internal/Patches/PuddleCubemapsPatch.hpp"
// warnings
#include "Internal/Warnings/DupeAddonNodesWarning.hpp"
#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"

namespace Internal::Messaging
{
	// handles various F4SE callback events
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
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
				// ActorCauseSaveBloatFix
				Internal::Fixes::ActorCauseSaveBloatFix::Install();

				// CombatMusicFix
				if (Internal::Fixes::CombatMusicFix::NeedsFix()) {
					Internal::Fixes::CombatMusicFix::Fix();
				}

				// WorkbenchSoundFix
				Internal::Fixes::WorkbenchSoundFix::Install();

				// StolenPowerArmorOwnershipFix
				Internal::Fixes::StolenPowerArmorOwnershipFix::Install();

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

				// PuddleCubemapsPatch
				Internal::Patches::PuddleCubemapsPatch::Install();

				// DupeAddonNodesWarnings
				Internal::Warnings::DupeAddonNodesWarning::Install();

				// LeveledListEntryCountWarning
				Internal::Warnings::LeveledListEntryCountWarning::Install();

				break;
			}
			default: {
				break;
			}
		}
	}
}

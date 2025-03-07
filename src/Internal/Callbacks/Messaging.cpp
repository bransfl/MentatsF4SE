#include "Internal/Fixes/CombatMusicFix.hpp"
// #include "Internal/Fixes/GrenadeMovementSpeedFix.hpp"
#include "Internal/Fixes/ActorCauseSaveBloatFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/StolenPowerArmorOwnershipFix.hpp"
#include "Internal/Fixes/WorkbenchSoundFix.hpp"
#include "Internal/Warnings/DupeAddonNodesWarning.hpp"
#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"
// #include "Internal/Warnings/RogueMasterWarning.hpp"

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

				// GrenadeMovementSpeedFix
				// Internal::Fixes::GrenadeMovementSpeedFix::Install();

				// WorkbenchSoundFix
				Internal::Fixes::WorkbenchSoundFix::Install();
				Internal::Fixes::WorkbenchSoundFix::FixWorkbenchSounds(RE::PlayerCharacter::GetSingleton());

				// StolenPowerArmorOwnershipFix
				Internal::Fixes::StolenPowerArmorOwnershipFix::Install();

				// DupeAddonNodesWarnings
				Internal::Warnings::DupeAddonNodesWarning::ClearNodeMap();

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

				// DupeAddonNodesWarnings
				Internal::Warnings::DupeAddonNodesWarning::Install();

				// LeveledListEntryCountWarning
				Internal::Warnings::LeveledListEntryCountWarning::CheckLeveledLists();

				// RogueMasterWarning
				// Internal::Warnings::RogueMasterWarning::Install();

				break;
			}
			default: {
				break;
			}
		}
	}
}

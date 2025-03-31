#include "Internal/Messaging.hpp"
// modules
#include "Internal/Fixes/ActorCauseSaveBloatFix.hpp"
#include "Internal/Fixes/CombatMusicFix.hpp"
#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Fixes/StolenPowerArmorOwnershipFix.hpp"
#include "Internal/Fixes/WorkbenchSoundFix.hpp"
#include "Internal/Patches/PuddleCubemapsPatch.hpp"
#include "Internal/Warnings/DupeAddonNodesWarning.hpp"
#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"

namespace Internal
{
	void Messaging::RegisterCallback()
	{
		logger::info("Registering for F4SE events..."sv);

		F4SE::GetMessagingInterface()->RegisterListener(Messaging::Callback);

		logger::info("Registered for F4SE events."sv);
	}

	// handles various F4SE callback events
	void Messaging::Callback(F4SE::MessagingInterface::Message* a_msg) noexcept
	{
		switch (a_msg->type) {
			case F4SE::MessagingInterface::kPostLoadGame: {
				logger::info("Internal::Messaging::Callback() recieved MessagingInterface::kPostLoadGame"sv);

				Internal::Fixes::ActorCauseSaveBloatFix::Install();

				if (Internal::Fixes::CombatMusicFix::NeedsFix()) {
					Internal::Fixes::CombatMusicFix::Fix();
				}

				Internal::Fixes::WorkbenchSoundFix::Install();

				Internal::Fixes::StolenPowerArmorOwnershipFix::Install();

				logger::info("Internal::Messaging::Callback() finished processing MessagingInterface::kPostLoadGame"sv);

				break;
			}
			case F4SE::MessagingInterface::kGameDataReady: {
				logger::info("Internal::Messaging::Callback() recieved MessagingInterface::kGameDataReady"sv);

				Internal::Fixes::CombatMusicFix::Install();

				Internal::Patches::PuddleCubemapsPatch::Install();

				Internal::Warnings::DupeAddonNodesWarning::Install();

				Internal::Warnings::LeveledListEntryCountWarning::Install();

				logger::info("Internal::Messaging::Callback() finished processing MessagingInterface::kGameDataReady"sv);

				break;
			}
			default: {
				break;
			}
		}
	}
}

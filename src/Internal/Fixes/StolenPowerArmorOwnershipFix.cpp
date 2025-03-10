#include "Internal/Fixes/StolenPowerArmorOwnershipFix.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility.hpp"

namespace Internal::Fixes
{
	static inline constexpr std::string_view command = "SetOwnership"; // passing without params sets the player as the owner

	void StolenPowerArmorOwnershipFix::Install() noexcept
	{
		logger::info("StolenPowerArmorOwnershipFix -> Fix installing..."sv);

		if (!Config::bStolenPowerArmorOwnershipFix.GetValue()) {
			logger::info("Fix aborted: StolenPowerArmorOwnershipFix. Reason: Fix was disabled in config file."sv);
			return;
		}

		RE::TESFurnitureEvent::GetEventSource()->RegisterSink(StolenPowerArmorOwnershipFix::FurnitureEventHandler::GetSingleton());

		logger::info("StolenPowerArmorOwnershipFix -> Fix installed."sv);
	}

	void StolenPowerArmorOwnershipFix::FixOwnership(RE::TESObjectREFR* a_ref)
	{
		if (!a_ref) {
			return;
		}

		Utility::ExecuteCommand(command, a_ref, true);
	}
}

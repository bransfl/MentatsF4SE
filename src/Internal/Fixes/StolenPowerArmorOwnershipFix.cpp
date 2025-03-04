#include "Internal/Fixes/StolenPowerArmorOwnershipFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

namespace Internal::Fixes
{
	const std::string_view command = "SetOwnership"; // passing without params sets the player as the owner

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

	bool StolenPowerArmorOwnershipFix::IsPowerArmor(RE::TESObjectREFR* a_ref)
	{
		if (a_ref->extraList.get()->HasType(RE::EXTRA_DATA_TYPE::kInactivePowerArmor)) {
			return true;
		}
		else if (a_ref->extraList.get()->HasType(RE::EXTRA_DATA_TYPE::kPowerArmor)) {
			return true;
		}
		return false;
	}
}

#include "Internal/Fixes/StolenPowerArmorOwnershipFix.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility.hpp"

namespace Internal::Fixes
{
	// passing this command without params sets the player as the owner
	static inline constexpr std::string_view command = "SetOwnership";

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

	void StolenPowerArmorOwnershipFix::FixOwnership(RE::TESObjectREFR* a_powerArmorRef)
	{
		if (!a_powerArmorRef) {
			return;
		}

		Utility::ExecuteCommand(command, a_powerArmorRef, true);
	}

	RE::BSEventNotifyControl StolenPowerArmorOwnershipFix::FurnitureEventHandler::ProcessEvent(const RE::TESFurnitureEvent& a_event, RE::BSTEventSource<RE::TESFurnitureEvent>*)
	{
		auto* player = RE::PlayerCharacter::GetSingleton();

		if (!a_event.actor.get() || a_event.actor.get() != player) {
			return RE::BSEventNotifyControl::kContinue;
		}

		RE::TESObjectREFR* furn = a_event.targetFurniture.get();
		if (!furn) {
			return RE::BSEventNotifyControl::kContinue;
		}

		// just in case the player hasnt used power armor yet
		if (!player->lastUsedPowerArmor.get() || !player->lastUsedPowerArmor.get().get()) {
			return RE::BSEventNotifyControl::kContinue;
		}

		if (furn == player->lastUsedPowerArmor.get().get()) {
			logger::info("StolenPowerArmorOwnershipFix -> FixOwnership() running on furniture: {}."sv,
				Utility::GetFormInfo(furn->GetBaseObject()));
			FixOwnership(furn);
		}

		return RE::BSEventNotifyControl::kContinue;
	}
}

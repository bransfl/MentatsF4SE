#include "Internal/Fixes/GrenadeMovementSpeedFix.hpp"
#include "Internal/Config/Config.hpp"

// unequips and reequips the player's grenade on save load to fix a semi-common movement speed bug
namespace Internal::Fixes::GrenadeMovementSpeedFix
{
	void Install() noexcept
	{
		logger::info("Fix installing: GrenadeMovementSpeedFix.");

		if (!Config::bGrenadeMovementSpeedFix.GetValue()) {
			logger::info("Fix aborted: GrenadeMovementSpeedFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		Fix();

		logger::info("Fix installed: GrenadeMovementSpeedFix.");
	}

	void Fix()
	{
		auto* player = RE::PlayerCharacter::GetSingleton();
		//  on save load:

		// check if grenade bipedslot has an item and player has >0 of the grenades in their inventory
		bool hasGrenade = false;
		player->currentProcess->middleHigh->equippedItemsLock.lock();
		for (auto it = player->currentProcess->middleHigh->equippedItems.begin(); it != player->currentProcess->middleHigh->equippedItems.end(); ++it) {
			if (it->equipIndex.index == 2) {
				hasGrenade = true;
				break;
			}
		}
		logger::info("GrenadeMovementSpeedFix -> HasGrenade: {}.", hasGrenade);
		if (!hasGrenade) {
			return;
		}

		RE::EquippedItem& equippedGrenadeItem = player->currentProcess->middleHigh->equippedItems[2];
		RE::TESObjectWEAP* equippedGrenadeForm = equippedGrenadeItem.item.object->As<RE::TESObjectWEAP>();

		uint32_t count = 0; // need to test the count + countComponent params, no idea what this is for
		int grenadeCount = player->GetItemCount(count, equippedGrenadeForm, false);
		logger::info("GrenadeMovementSpeedFix -> grenadeCount: {}.", grenadeCount);
		if (grenadeCount == 0) {
			return;
		}

		// auto* equipManager = RE::ActorEquipManager::GetSingleton();
		//  unequip the grenade item
		// equipManager->UnequipObject();

		// reequip the grenade item
		// equipManager->EquipObject();
	}
}

// bool UnequipObject(
// 	Actor* a_actor,
// 	const BGSObjectInstance* a_object,
// 	std::uint32_t a_number,
// 	const BGSEquipSlot* a_slot,
// 	std::uint32_t a_stackID,
// 	bool a_queueEquip,
// 	bool a_forceEquip,
// 	bool a_playSounds,
// 	bool a_applyNow,
// 	const BGSEquipSlot* a_slotBeingReplaced)

// bool EquipObject(
// 	Actor* a_actor,
// 	const BGSObjectInstance& a_object,
// 	std::uint32_t a_stackID,
// 	std::uint32_t a_number,
// 	const BGSEquipSlot* a_slot,
// 	bool a_queueEquip,
// 	bool a_forceEquip,
// 	bool a_playSounds,
// 	bool a_applyNow,
// 	bool a_locked)

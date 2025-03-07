#include "Internal/Fixes/GrenadeMovementSpeedFix.hpp"
#include "Internal/Config/Config.hpp"

// unequips and reequips the player's grenade on save load to fix a semi-common movement speed bug
namespace Internal::Fixes
{
	// RE::BGSEquipSlot* grenadeSlot;
	// grenadeSlot = (RE::BGSEquipSlot*)RE::TESForm::GetFormByID(0x46AAC);

	void GrenadeMovementSpeedFix::Install() noexcept
	{
		logger::info(FMT_STRING("Fix installing: GrenadeMovementSpeedFix."sv));

		if (!Config::bGrenadeMovementSpeedFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: GrenadeMovementSpeedFix. Reason: Fix was disabled in config file."sv));
			return;
		}

		Fix();

		logger::info(FMT_STRING("Fix installed: GrenadeMovementSpeedFix."sv));
	}

	void GrenadeMovementSpeedFix::Fix()
	{
		// TODO NOT FINISHED YET

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}

		// check if grenade bipedslot has an item and player has >0 of the grenades in their inventory
		bool hasGrenade = false;
		player->currentProcess->middleHigh->equippedItemsLock.lock();
		for (auto it = player->currentProcess->middleHigh->equippedItems.begin(); it != player->currentProcess->middleHigh->equippedItems.end(); ++it) {
			if (it->equipIndex.index == 2) {
				hasGrenade = true;
				break;
			}
		}
		player->currentProcess->middleHigh->equippedItemsLock.unlock();

		logger::info(FMT_STRING("GrenadeMovementSpeedFix -> HasGrenade: {}."sv), hasGrenade);
		if (!hasGrenade) {
			return;
		}

		const RE::EquippedItem& equippedGrenadeItem = player->currentProcess->middleHigh->equippedItems[2];
		RE::TESForm* grenadeForm = equippedGrenadeItem.item.object;
		if (!grenadeForm) {
			logger::info(FMT_STRING("GrenadeMovementSpeedFix -> grenadeForm was none"sv));
			return;
		}
		RE::TESObjectWEAP* grenadeWeapon = grenadeForm->As<RE::TESObjectWEAP>();
		if (!grenadeWeapon) {
			logger::info(FMT_STRING("GrenadeMovementSpeedFix -> grenadeWeapon was none"sv));
			return;
		}

		uint32_t grenadeCount = 0;
		player->GetItemCount(grenadeCount, grenadeWeapon, false);

		auto* equipManager = RE::ActorEquipManager::GetSingleton();
		if (!equipManager) {
			return;
		}

		logger::info(FMT_STRING("GrenadeMovementSpeedFix -> Unequipping grenade..."sv));
		equipManager->UnequipObject(
			/* a_actor */ player,
			/* a_object */ &equippedGrenadeItem.item,
			/* a_number */ grenadeCount,
			/* a_slot */ equippedGrenadeItem.equipSlot,
			/* a_stackID */ 0,
			/* a_queueEquip */ true,
			/* a_forceEquip */ true,
			/* a_playSounds */ false,
			/* a_applyNow */ true,
			/* a_slotBeingReplaced */ nullptr);
		logger::info(FMT_STRING("GrenadeMovementSpeedFix -> Unequipped grenade."sv));
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

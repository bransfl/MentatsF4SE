#include "Internal/Fixes/GrenadeMovementSpeedFix.hpp"
#include "Internal/Config/Config.hpp"

// unequips and reequips the player's grenade on save load to fix a semi-common movement speed bug
namespace Internal::Fixes::GrenadeMovementSpeedFix
{
	RE::BGSEquipSlot* grenadeSlot;

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
		// on save load

		// auto* player = RE::PlayerCharacter::GetSingleton();
		// if (!player) {
		// 	return;
		// }

		// if(!player->currentProcess->middleHigh) {
		// 	return;
		// }

		// // check if grenade bipedslot has an item and player has >0 of the grenades in their inventory
		// bool hasGrenade = false;
		// for (auto it = player->currentProcess->middleHigh->equippedItems.begin(); it != player->currentProcess->middleHigh->equippedItems.end(); ++it) {
		// 	if (it->equipIndex.index == 2) {
		// 		hasGrenade = true;
		// 		break;
		// 	}
		// }
		// logger::info("GrenadeMovementSpeedFix -> HasGrenade: {}.", hasGrenade);
		// if (!hasGrenade) {
		// 	return;
		// }

		// //grenadeSlot = (RE::BGSEquipSlot*)RE::TESForm::GetFormByID(0x46AAC);

		// const RE::EquippedItem& equippedGrenadeItem = player->currentProcess->middleHigh->equippedItems[2];
		// RE::TESObjectWEAP* equippedGrenadeForm = equippedGrenadeItem.item.object->As<RE::TESObjectWEAP>();
		// //logger::info(FMT_STRING("equippedGrenadeForm: {}"), equippedGrenadeForm->GetFormEditorID());

		// uint32_t grenadeCount;
		// player->GetItemCount(grenadeCount, equippedGrenadeForm, true);
		// logger::info("GrenadeMovementSpeedFix - GrenadeCount = {}", grenadeCount);
		// if(grenadeCount == 0) {
		// 	return;
		// }

		// auto* equipManager = RE::ActorEquipManager::GetSingleton();
		// if(!equipManager) {
		// 	return;
		// }

		// // bool bUnequip = equipManager->UnequipObject(
		// // 	player,
		// // 	&equippedGrenadeItem.item,
		// // 	grenadeCount,
		// // 	equippedGrenadeItem.equipSlot,
		// // 	0,
		// // 	true,
		// // 	true,
		// // 	true,
		// // 	true,
		// // 	grenadeSlot);
		// // logger::info("GrenadeMovementSpeedFix = bUnequip = {}", bUnequip);
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

#pragma once

namespace Internal::Fixes
{
	class StolenPowerArmorOwnershipFix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Gives the player ownership of the given PowerArmor reference.
		 * @param a_ref The PowerArmor to patch.
		 */
		static void FixOwnership(RE::TESObjectREFR* a_ref);

		class FurnitureEventHandler : public RE::BSTEventSink<RE::TESFurnitureEvent>
		{
		public:
			[[nodiscard]] static FurnitureEventHandler* GetSingleton()
			{
				static FurnitureEventHandler singleton;
				return std::addressof(singleton);
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent& a_event, RE::BSTEventSource<RE::TESFurnitureEvent>*) override
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
					logger::info("StolenPowerArmorOwnershipFix -> FixOwnership() running on furniture: (FormID: {:08X}, EditorID: {})."sv,
						furn->GetFormID(), furn->GetBaseObject()->GetFormEditorID());
					FixOwnership(furn);
				}

				return RE::BSEventNotifyControl::kContinue;
			}

			FurnitureEventHandler() = default;
			FurnitureEventHandler(const FurnitureEventHandler&) = delete;
			FurnitureEventHandler(FurnitureEventHandler&&) = delete;
			~FurnitureEventHandler() = default;
			FurnitureEventHandler& operator=(const FurnitureEventHandler&) = delete;
			FurnitureEventHandler& operator=(FurnitureEventHandler&&) = delete;
		};
	};
}

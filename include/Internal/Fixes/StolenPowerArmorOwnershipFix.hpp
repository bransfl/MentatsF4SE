#pragma once

namespace Internal::Fixes
{
	class StolenPowerArmorOwnershipFix
	{
	public:
		static void Install() noexcept;

	private:
		static void FixOwnership(RE::TESObjectREFR* a_ref);

		static bool IsPowerArmor(RE::TESObjectREFR* a_ref);

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
				logger::info("StolenPowerArmorOwnershipFix -> TESFurnitureEvent receieved"sv);

				if (!a_event.actor || a_event.actor.get()) {
					return RE::BSEventNotifyControl::kContinue;
				}

				if (a_event.actor.get() != RE::PlayerCharacter::GetSingleton()) {
					return RE::BSEventNotifyControl::kContinue;
				}

				RE::TESObjectREFR* furn = a_event.targetFurniture.get();
				if (!furn) {
					return RE::BSEventNotifyControl::kContinue;
				}

				// when the player enters or exits a set of power armor, set the player as the owner
				if (IsPowerArmor(furn)) {
					if (furn->GetHasOwner() && furn->GetOwner() != RE::PlayerCharacter::GetSingleton()) {
						FixOwnership(furn);
					}
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

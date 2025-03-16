#pragma once

// Sounds:
// UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
// UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
// UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

namespace Internal::Fixes
{
	class WorkbenchSoundFix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

		/**
		 * @brief Stops all workbench SFX annotations on the given reference.
		 * @param a_workbenchUser The reference to silence.
		 */
		static void FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser);

		/**
		 * @brief Checks if the given furniture reference is a workbench.
		 * @param a_furniture The reference to check.
		 * @return If the a_furniture is a workbench.
		 *		   If the reference is null, this returns false.
		 */
		static bool IsWorkbench(RE::TESFurniture* a_furniture);

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
				if (a_event.IsExit()) {
					RE::TESFurniture* a_furniture = a_event.targetFurniture.get()->As<RE::TESFurniture>();

					if (!a_furniture || !IsWorkbench(a_furniture)) {
						return RE::BSEventNotifyControl::kContinue;
					}

					RE::TESObjectREFR* actor = a_event.actor.get();
					if (!actor) {
						return RE::BSEventNotifyControl::kContinue;
					}

					FixWorkbenchSounds(actor);
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

		class ActorCellEventHandler : public RE::BSTEventSink<RE::BGSActorCellEvent>
		{
		public:
			[[nodiscard]] static ActorCellEventHandler* GetSingleton()
			{
				static ActorCellEventHandler singleton;
				return std::addressof(singleton);
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent& a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) override
			{
				if (a_event.actor.get().get() && a_event.actor.get().get() != RE::PlayerCharacter::GetSingleton()) {
					return RE::BSEventNotifyControl::kContinue;
				}

				auto ui = RE::UI::GetSingleton();
				if (!ui) {
					return RE::BSEventNotifyControl::kContinue;
				}
				if (ui->GetMenuOpen("ExamineMenu"sv) || ui->GetMenuOpen("CookingMenu"sv)) {
					return RE::BSEventNotifyControl::kContinue;
				}

				FixWorkbenchSounds(a_event.actor.get().get());

				return RE::BSEventNotifyControl::kContinue;
			}

			ActorCellEventHandler() = default;
			ActorCellEventHandler(const ActorCellEventHandler&) = delete;
			ActorCellEventHandler(ActorCellEventHandler&&) = delete;
			~ActorCellEventHandler() = default;
			ActorCellEventHandler& operator=(const ActorCellEventHandler&) = delete;
			ActorCellEventHandler& operator=(ActorCellEventHandler&&) = delete;
		};
	};
}

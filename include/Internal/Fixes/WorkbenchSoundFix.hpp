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
		static void Install() noexcept;

		static void FixWorkbenchSound();

		// checks if the given furniture is a valid workbench
		static bool IsWorkbench(RE::TESFurniture* a_furniture);

		static bool IsPlayerCompanion(RE::Actor* a_actor);

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
				logger::info("WorkbenchSoundFix -> TESFurnitureEvent receieved"sv);

				if (a_event.IsExit()) {
					RE::TESFurniture* a_furniture = a_event.targetFurniture.get()->As<RE::TESFurniture>();
					if (!a_furniture)
						return RE::BSEventNotifyControl::kContinue;

					bool bIsWorkbench = IsWorkbench(a_furniture);
					if (!bIsWorkbench)
						return RE::BSEventNotifyControl::kContinue;

					FixWorkbenchSound();
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
				logger::info("WorkbenchSoundFix -> ActorCellEvent receieved"sv);

				auto eventActor = a_event.actor.get().get();
				if (eventActor != RE::PlayerCharacter::GetSingleton()) {
					return RE::BSEventNotifyControl::kContinue;
				}

				auto ui = RE::UI::GetSingleton();
				if (!ui) {
					return RE::BSEventNotifyControl::kContinue;
				}
				if (ui->GetMenuOpen("ExamineMenu"sv) || ui->GetMenuOpen("CookingMenu"sv)) {
					logger::info("WorkbenchSoundFix -> ui had relevant menu open, return"sv);
					return RE::BSEventNotifyControl::kContinue;
				}

				FixWorkbenchSound();

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

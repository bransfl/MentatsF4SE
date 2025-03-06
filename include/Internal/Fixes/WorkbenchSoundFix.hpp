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
		// installs the fix
		static void Install() noexcept;

		// stops the goddamned sewing machine sound
		static void FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser);

		// checks if the given furniture is a valid workbench
		static bool IsWorkbench(RE::TESFurniture* a_furniture);

		// checks if the given actor is a valid companion - unused
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
				if (a_event.IsExit()) {
					RE::TESFurniture* a_furniture = a_event.targetFurniture.get()->As<RE::TESFurniture>();

					if (!a_furniture || !IsWorkbench(a_furniture)) {
						return RE::BSEventNotifyControl::kContinue;
					}

					if (a_event.actor.get()) {
						FixWorkbenchSounds(a_event.actor.get());
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

				if (a_event.actor.get().get() != RE::PlayerCharacter::GetSingleton()) {
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

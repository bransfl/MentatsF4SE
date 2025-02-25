#pragma once

// Sounds:
// UIWorkshopSewingMachineRunLPM [SNDR:0019E999]
// UIWorkshopPowerArmorWeldLPM [SNDR:0022C1D3]
// UIWorkshopDrillPressDrillLPM [SNDR:0017C8D2]
// UIWorkshopDrillPressPowerLPM [SNDR:0017C8D3]

// Workbenches:
// workbenchWeaponsA "Weapons Workbench" [FURN:0017B3A4]
// workbenchWeaponsB "Weapons Workbench" [FURN:0017E787]
// WorkbenchPowerArmor "Power Armor Station" [FURN:00157FEB]
// WorkbenchPowerArmorSmall "Power Armor Station" [FURN:0013BD08]
// WorkbenchArmorA "Armor Workbench" [FURN:0012EA9B]
// WorkshopScavengingStation "Scavenging Station" [FURN:0008674C]

// not finished or working consistently, disabled for now
namespace Internal::Fixes::WorkbenchSoundFix
{
	inline constexpr std::string_view examineMenu = "ExamineMenu";

	void Install() noexcept;

	void KillSoundsAll();

	void KillSoundSewingMachine();
	void KillSoundWeld();
	void KillSoundPressDrill();
	void KillSoundPressPower();

	std::vector<RE::TESObjectREFR*> GetFurnitureRefsInCell(RE::TESObjectCELL* a_cell);

	// checks if the given furniture is a valid workbench and optionally kills relevant sound
	bool CheckWorkbench(RE::TESObjectREFR* furniture, bool killSound);

	namespace Events
	{
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
					logger::info("WorkbenchSoundFix -> TESFurnitureEvent receieved, was exit type"sv);

					bool bChecked = CheckWorkbench(a_event.targetFurniture.get(), true);
					if (bChecked) {
						logger::info("WorkbenchSoundFix -> bChecked = true");
					}
					else {
						logger::info("WorkbenchSoundFix -> bChecked = false");
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

				auto eventActor = a_event.actor.get().get();
				if (eventActor != RE::PlayerCharacter::GetSingleton()) {
					logger::info("WorkbenchSoundFix -> actor was not player - return"sv);
					return RE::BSEventNotifyControl::kContinue;
				}
				logger::info("WorkbenchSoundFix -> actor was player"sv);

				auto ui = RE::UI::GetSingleton();
				if (!ui) {
					logger::info("WorkbenchSoundFix -> ui was null - return"sv);
					return RE::BSEventNotifyControl::kContinue;
				}

				// ExamineMenu is weapons, armor, and power armor workbenches, as far as I know.
				// CookingMenu is when you access the Chem Workbench or Cooking stations.
				if (ui->GetMenuOpen(examineMenu)) {
					logger::info("WorkbenchSoundFix -> ExamineMenu was not open - return"sv);
					return RE::BSEventNotifyControl::kContinue;
				}

				KillSoundsAll();

				return RE::BSEventNotifyControl::kContinue;
			}

			ActorCellEventHandler() = default;
			ActorCellEventHandler(const ActorCellEventHandler&) = delete;
			ActorCellEventHandler(ActorCellEventHandler&&) = delete;
			~ActorCellEventHandler() = default;
			ActorCellEventHandler& operator=(const ActorCellEventHandler&) = delete;
			ActorCellEventHandler& operator=(ActorCellEventHandler&&) = delete;
		};
	}
}
// auto ui = RE::UI::GetSingleton();
// 			if (ui && (ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME)
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

namespace Internal::Fixes::WorkbenchSoundFix
{
	void Install() noexcept;

	void FixWorkbenchSound(RE::TESFurniture* a_workbench);

	std::vector<RE::TESObjectREFR*> GetFurnitureRefsInCell(RE::TESObjectCELL* a_cell);

	// checks if the given furniture is a valid workbench
	bool IsWorkbench(RE::TESFurniture* a_furniture);

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

					RE::TESFurniture* a_furniture = a_event.targetFurniture.get()->As<RE::TESFurniture>();
					bool bChecked = IsWorkbench(a_furniture);
					if (bChecked) {
						logger::info("WorkbenchSoundFix -> bChecked = true"sv);
					}
					else {
						logger::info("WorkbenchSoundFix -> bChecked = false. return"sv);
						return RE::BSEventNotifyControl::kContinue;
					}

					FixWorkbenchSound(a_furniture);
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

				// disabled for now since it might not be necessary

				auto eventActor = a_event.actor.get().get();
				if (eventActor != RE::PlayerCharacter::GetSingleton()) {
					logger::info("WorkbenchSoundFix -> actor was not player - return"sv);
					return RE::BSEventNotifyControl::kContinue;
				}
				logger::info("WorkbenchSoundFix -> actor was player"sv);

				// auto ui = RE::UI::GetSingleton();
				// if (!ui) {
				// 	logger::info("WorkbenchSoundFix -> ui was null - return"sv);
				// 	return RE::BSEventNotifyControl::kContinue;
				// }

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

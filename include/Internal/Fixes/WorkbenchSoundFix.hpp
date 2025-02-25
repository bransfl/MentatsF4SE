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
				logger::info("TESFurnitureEvent receieved"sv);
				if (a_event.type == RE::TESFurnitureEvent::FurnitureEventType::kExit) {
					//
				}

				return RE::BSEventNotifyControl::kContinue;
			}

		private:
			FurnitureEventHandler() = default;
			FurnitureEventHandler(const FurnitureEventHandler&) = delete;
			FurnitureEventHandler(FurnitureEventHandler&&) = delete;
			~FurnitureEventHandler() = default;
			FurnitureEventHandler& operator=(const FurnitureEventHandler&) = delete;
			FurnitureEventHandler& operator=(FurnitureEventHandler&&) = delete;
		};
	}
}

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

	std::vector<RE::TESObjectREFR*> GetWorkbenchRefsInCell(RE::TESObjectCELL* a_cell);

	bool IsValidWorkbench(int32_t formID);

	void KillWorkbenchSound(RE::TESObjectREFR* workbench);

	namespace Events
	{
		namespace Cell
		{
			class CellAttachDetachEventHandler : public RE::BSTEventSink<RE::CellAttachDetachEvent>
			{
			public:
				[[nodiscard]] static CellAttachDetachEventHandler* GetSingleton()
				{
					static CellAttachDetachEventHandler singleton;
					return std::addressof(singleton);
				}

				RE::BSEventNotifyControl ProcessEvent(const RE::CellAttachDetachEvent& a_event, RE::BSTEventSource<RE::CellAttachDetachEvent>*) override
				{
					logger::info("CellAttachDetachEvent receieved"sv);

					// idea - on predetach:
					//	get all furn types, check for armor workbench
					//		if found, check if occupied
					//			if occupied, kill sound
					switch (*a_event.type) {
						case RE::CellAttachDetachEvent::EVENT_TYPE::kPreDetach: {
							std::vector<RE::TESObjectREFR*> workbenches = GetWorkbenchRefsInCell(a_event.cell);
							if (workbenches.size() > 0) {
								for (RE::TESObjectREFR* bench : workbenches) {
									KillWorkbenchSound(bench);
									logger::info("CellAttachDetachEvent sound killed"sv);
								}
							}
							break;
						}
						default: {
							break;
						}
					}

					return RE::BSEventNotifyControl::kContinue;
				}

			private:
				CellAttachDetachEventHandler() = default;
				CellAttachDetachEventHandler(const CellAttachDetachEventHandler&) = delete;
				CellAttachDetachEventHandler(CellAttachDetachEventHandler&&) = delete;
				~CellAttachDetachEventHandler() = default;
				CellAttachDetachEventHandler& operator=(const CellAttachDetachEventHandler&) = delete;
				CellAttachDetachEventHandler& operator=(CellAttachDetachEventHandler&&) = delete;
			};
		}

		namespace Furniture
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
					if (a_event.IsExit()) {
						RE::TESObjectREFR* bench = a_event.targetFurniture.get();
						KillWorkbenchSound(bench);
						logger::info("TESFurnitureEvent sound killed"sv);
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
}

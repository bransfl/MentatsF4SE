#pragma once

namespace Internal::Fixes::SewingMachineSoundFix
{
	void Install() noexcept;

	void KillSound();

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
					// idea - on predetach:
					//	get all furn types, check for armor workbench
					//		if found, check if occupied
					//			if occupied, kill sound
					switch (*a_event.type) {
						case RE::CellAttachDetachEvent::EVENT_TYPE::kPreDetach: {
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
					if (a_event.IsExit()) {
						// temp code for compiler
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

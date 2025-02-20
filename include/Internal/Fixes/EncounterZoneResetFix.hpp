#pragma once

namespace Internal::Fixes::EncounterZoneResetFix
{
	void Install() noexcept;

	namespace Events
	{
		class CellAttachDetachEventHandler : public RE::BSTEventSink<RE::CellAttachDetachEvent>
		{
		public:
			[[nodiscard]] static CellAttachDetachEventHandler* GetSingleton()
			{
				static CellAttachDetachEventHandler singleton;
				return std::addressof(singleton);
			}

		private:
			CellAttachDetachEventHandler() = default;
			CellAttachDetachEventHandler(const CellAttachDetachEventHandler&) = delete;
			CellAttachDetachEventHandler(CellAttachDetachEventHandler&&) = delete;
			~CellAttachDetachEventHandler() = default;
			CellAttachDetachEventHandler& operator=(const CellAttachDetachEventHandler&) = delete;
			CellAttachDetachEventHandler& operator=(CellAttachDetachEventHandler&&) = delete;

			RE::BSEventNotifyControl ProcessEvent(const RE::CellAttachDetachEvent& a_event, RE::BSTEventSource<RE::CellAttachDetachEvent>*) override
			{
				switch (*a_event.type) {
					case RE::CellAttachDetachEvent::EVENT_TYPE::kPreDetach: {
						const auto cell = a_event.cell;
						const auto encZone = cell ? cell->GetEncounterZone() : nullptr;
						const auto calendar = RE::Calendar::GetSingleton();
						if (encZone && calendar) {
							uint32_t detachTimeNew = static_cast<uint32_t>(calendar->GetHoursPassed());
							encZone->gameData.detachTime = detachTimeNew;
						}
						break;
					}
					default: {
						break;
					}
				}
				return RE::BSEventNotifyControl::kContinue;
			}
		};
	}
}
// Sink() = default;
// Sink(const Sink&) = delete;
// Sink(Sink&&) = delete;
// ~Sink() = default;
// Sink& operator=(const Sink&) = delete;
// Sink& operator=(Sink&&) = delete;
// RE::BSEventNotifyControl ProcessEvent(const RE::CellAttachDetachEvent& a_event, RE::BSTEventSource<RE::CellAttachDetachEvent>*) override
// 			{
// 				switch (*a_event.type) {
// 				case RE::CellAttachDetachEvent::EVENT_TYPE::kPreDetach:
// 					{
// 						const auto cell = a_event.cell;
// 						const auto ez = cell ? cell->GetEncounterZone() : nullptr;
// 						const auto calendar = RE::Calendar::GetSingleton();
// 						if (ez && calendar) {
// 							ez->SetDetachTime(
// 								static_cast<std::uint32_t>(calendar->GetHoursPassed()));
// 						}
// 					}
// 					break;
// 				default:
// 					break;
// 				}

// 				return RE::BSEventNotifyControl::kContinue;
// 			}
// 	}
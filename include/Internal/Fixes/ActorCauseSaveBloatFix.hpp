#pragma once

namespace Internal::Fixes
{
	class ActorCauseSaveBloatFix
	{
	public:
		static void Install() noexcept;

	private:
		static std::vector<RE::TESObjectREFR*> GetProjectilesInCell(RE::TESObjectCELL* a_cell);

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
				switch (*a_event.type) {
					case RE::CellAttachDetachEvent::EVENT_TYPE::kPreDetach: {
						std::vector<RE::TESObjectREFR*> projectiles = GetProjectilesInCell(a_event.cell);
						size_t count = projectiles.size();
						logger::info(FMT_STRING("ActorCauseSaveBloatFix -> (tempmesg) projectiles count: {}"), count);
						if (count == 0) {
							break;
						}
						else {
							for (RE::TESObjectREFR* proj : projectiles) {
								if (proj->GetActorCause() != nullptr) {
									proj->SetActorCause(nullptr);
									logger::info(FMT_STRING("ActorCauseSaveBloatFix -> (tempmesg) removed actorcause for editorid: {}, formid: {:08X}"),
										proj->GetBaseObject()->GetFormEditorID(), proj->GetFormID());
								}
								else {
									logger::info(FMT_STRING("ActorCauseSaveBloatFix -> (tempmesg) did not have to remove actorcause for editorid: {}, formid: {:08X}"),
										proj->GetBaseObject()->GetFormEditorID(), proj->GetFormID());
								}
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
	};
}

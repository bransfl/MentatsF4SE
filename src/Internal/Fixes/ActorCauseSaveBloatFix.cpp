#include "Internal/Fixes/ActorCauseSaveBloatFix.hpp"
#include "Internal/Config.hpp"

namespace Internal::Fixes
{
	void ActorCauseSaveBloatFix::Install() noexcept
	{
		logger::info("ActorCauseSaveBloatFix -> Fix installing..."sv);

		if (!Config::bActorCauseSaveBloatFix.GetValue()) {
			logger::info("Fix aborted: ActorCauseSaveBloatFix. Reason: Fix was disabled in config file."sv);
			return;
		}

		auto& cells = RE::CellAttachDetachEventSource::CellAttachDetachEventSourceSingleton::GetSingleton();
		cells.source.RegisterSink(ActorCauseSaveBloatFix::CellAttachDetachEventHandler::GetSingleton());

		logger::info("ActorCauseSaveBloatFix -> Fix installed."sv);
	}

	std::vector<RE::TESObjectREFR*> ActorCauseSaveBloatFix::GetProjectilesInCell(RE::TESObjectCELL* a_cell)
	{
		auto refs = std::vector<RE::TESObjectREFR*>();

		if (!a_cell) {
			return refs;
		}

		a_cell->ForEachRef([&](RE::TESObjectREFR* a_ref) {
			if (a_ref) {
				if (a_ref->GetBaseObject()->formType == RE::ENUM_FORMTYPE::kPROJ) {
					refs.push_back(a_ref);
				}
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});

		return refs;
	}

	RE::BSEventNotifyControl ActorCauseSaveBloatFix::CellAttachDetachEventHandler::ProcessEvent(const RE::CellAttachDetachEvent& a_event, RE::BSTEventSource<RE::CellAttachDetachEvent>*)
	{
		switch (*a_event.type) {
			case RE::CellAttachDetachEvent::EVENT_TYPE::kPreDetach: {
				if (!a_event.cell) {
					logger::warn("ActorCauseSaveBloatFix -> a_event.cell was nullptr. Skipping this cell."sv);
				}

				std::vector<RE::TESObjectREFR*> projectiles = GetProjectilesInCell(a_event.cell);
				logger::info("ActorCauseSaveBloatFix -> Processing projectiles vector. Size: {}."sv, projectiles.size());
				if (projectiles.size() == 0) {
					break;
				}
				else {
					for (RE::TESObjectREFR* proj : projectiles) {
						if (proj) {
							if (proj->GetActorCause() != nullptr) {
								proj->SetActorCause(nullptr);
							}
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
}

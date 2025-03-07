#include "Internal/Fixes/ActorCauseSaveBloatFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes
{
	void ActorCauseSaveBloatFix::Install() noexcept
	{
		logger::info(FMT_STRING("ActorCauseSaveBloatFix -> Fix installing..."sv));

		if (!Config::bActorCauseSaveBloatFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: ActorCauseSaveBloatFix. Reason: Fix was disabled in config file."sv));
			return;
		}

		auto& cells = RE::CellAttachDetachEventSource::CellAttachDetachEventSourceSingleton::GetSingleton();
		cells.source.RegisterSink(ActorCauseSaveBloatFix::CellAttachDetachEventHandler::GetSingleton());

		logger::info(FMT_STRING("ActorCauseSaveBloatFix -> Fix installed."sv));
	}

	std::vector<RE::TESObjectREFR*> ActorCauseSaveBloatFix::GetProjectilesInCell(RE::TESObjectCELL* a_cell)
	{
		auto refs = std::vector<RE::TESObjectREFR*>();
		if (!a_cell) {
			return refs;
		}

		a_cell->ForEachRef([&](RE::TESObjectREFR* a_ref) {
			if (a_ref && a_ref->GetBaseObject()->formType == RE::ENUM_FORMTYPE::kPROJ) {
				refs.push_back(a_ref);
			}

			return RE::BSContainer::ForEachResult::kContinue;
		});
		return refs;
	}
}

#include "Internal/Fixes/CellAttachDetachEvent.hpp"
#include "RE/Bethesda/GameScript.hpp"
#include "RE/Bethesda/TESCellAttachDetachEvent.hpp"
#include "Internal/Config/Config.hpp"

// stolen geluxrum code from
// https://github.com/GELUXRUM/EngineFixesF4/blob/master/EngineFixesF4/src/main.cpp
namespace Internal::Fixes::CellAttachDetachEvent
{
	void Install() noexcept
	{
		if (!Config::bCellAttachDetachEvent.GetValue()) {
			logger::info("CellAttachDetachEvent -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}

		RE::RegisterForCellAttachDetach(CellAttachDetachListener::GetSingleton());
		logger::info("CellAttachDetachEvent -> Registered cell attach/detach listener.");
	}

	CellAttachDetachListener* CellAttachDetachListener::GetSingleton()
	{
		static CellAttachDetachListener instance;
		return &instance;
	}

	RE::BSEventNotifyControl Internal::Fixes::CellAttachDetachEvent::CellAttachDetachListener::ProcessEvent(const RE::TESCellAttachDetachEvent& a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*)
	{
		if (a_event.reference != nullptr && !a_event.attached) {
			a_event.reference->UpdateDynamicNavmesh(false);
			logger::debug("CellAttachDetachEvent -> DynamicNavmesh was updated.");
		}
		return RE::BSEventNotifyControl::kContinue;
	}
}

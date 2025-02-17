#include "RE/Bethesda/GameScript.hpp"
#include "RE/Bethesda/TESCellAttachDetachEvent.hpp"
#include "Internal/Fixes/CellAttachDetachEvent.hpp"

// stolen geluxrum code from
// https://github.com/GELUXRUM/EngineFixesF4/blob/master/EngineFixesF4/src/main.cpp
namespace Internal::Fixes::CellAttachDetachEvent
{
	void Install() noexcept
	{
		RE::RegisterForCellAttachDetach(CellAttachDetachListener::GetSingleton());
		logger::info("Registered cell attach/detach listener.");
	}


	CellAttachDetachListener* GetSingleton() {
		static CellAttachDetachListener instance;
		return &instance;
	}

	RE::BSEventNotifyControl ProcessEvent(const RE::TESCellAttachDetachEvent& a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*)
	{
		if (a_event.reference != nullptr && !a_event.attached) {
			a_event.reference->UpdateDynamicNavmesh(false);
		}
		return RE::BSEventNotifyControl::kContinue;
	}
}

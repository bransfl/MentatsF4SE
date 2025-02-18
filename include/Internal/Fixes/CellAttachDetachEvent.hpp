#pragma once

namespace Internal::Fixes::CellAttachDetachEvent
{
	void Install() noexcept;

	class CellAttachDetachListener : public RE::BSTEventSink<RE::TESCellAttachDetachEvent>
	{
	public:
		static CellAttachDetachListener* GetSingleton();
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESCellAttachDetachEvent& a_event, RE::BSTEventSource<RE::TESCellAttachDetachEvent>*) override;
	};
}

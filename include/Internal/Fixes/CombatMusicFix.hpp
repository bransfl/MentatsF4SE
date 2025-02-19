#pragma once

namespace Internal::Fixes::CombatMusicFix
{
	void Install() noexcept;

	void Fix();

	class TESDeathEventListener : public RE::BSTEventSink<RE::TESDeathEvent>
	{
	public:
		static TESDeathEventListener* GetSingleton();
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override;
	};
}

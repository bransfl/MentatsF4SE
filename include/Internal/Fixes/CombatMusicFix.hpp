#pragma once
#pragma warning(disable : 4100)

namespace Internal::Fixes::CombatMusicFix
{
	void Install() noexcept;

	void InitFix();

	void Fix();

	class EventSink : public RE::BSTEventSink<RE::TESDeathEvent>
	{
		EventSink() = default;

	public:
		EventSink(const EventSink&) = delete;
		EventSink(EventSink&&) = delete;
		EventSink& operator=(const EventSink&) = delete;
		EventSink& operator=(EventSink&&) = delete;

		static EventSink* GetSingleton()
		{
			static EventSink singleton;
			return &singleton;
		}

		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override
		{
			auto playerCharacter = RE::PlayerCharacter::GetSingleton();
			if (playerCharacter && !playerCharacter->IsInCombat()) {
				Fix();
			}
			return RE::BSEventNotifyControl::kContinue;
		}
	};
}

#pragma once
#pragma warning(push)
#pragma warning(disable : 4100) // for a_event

namespace Internal::Fixes::CombatMusicFix
{
	void Install() noexcept;

	void Fix();

	bool NeedsFix();

	namespace Events
	{
		class DeathEventHandler : public RE::BSTEventSink<RE::TESDeathEvent>
		{
		public:
			[[nodiscard]] static DeathEventHandler* GetSingleton()
			{
				static DeathEventHandler singleton;
				return std::addressof(singleton);
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override
			{
				logger::info("CombatMusicFix -> Event recieved.");

				if (NeedsFix()) {
					Fix();
				}

				return RE::BSEventNotifyControl::kContinue;
			}
		};
	}
}

#pragma warning(pop)

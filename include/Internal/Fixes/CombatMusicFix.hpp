#pragma once

namespace Internal::Fixes
{
	class CombatMusicFix
	{
	public:
		// installs the fix
		static void Install() noexcept;

		// stops combat music
		static void Fix();

		// checks if combat music needs to be stopped
		static bool NeedsFix();

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
				// if(a_event.dying == false) {
				// TODO - check if dying==false is dead or if dying==true is dead
				// return early if theyre dying. proceed if theyre dead.
				// }

				if (a_event.actorDying.get() == nullptr || a_event.actorKiller.get() == nullptr) {
					return RE::BSEventNotifyControl::kContinue;
				}

				if (a_event.actorKiller.get() != RE::PlayerCharacter::GetSingleton()) {
					// we dont need to run this for every single kill
					return RE::BSEventNotifyControl::kContinue;
				}

				if (NeedsFix()) {
					Fix();
				}

				return RE::BSEventNotifyControl::kContinue;
			}

		private:
			DeathEventHandler() = default;
			DeathEventHandler(const DeathEventHandler&) = delete;
			DeathEventHandler(DeathEventHandler&&) = delete;
			~DeathEventHandler() = default;
			DeathEventHandler& operator=(const DeathEventHandler&) = delete;
			DeathEventHandler& operator=(DeathEventHandler&&) = delete;
		};
	};
}

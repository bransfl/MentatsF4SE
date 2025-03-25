#pragma once

namespace Internal::Fixes
{
	class CombatMusicFix
	{
	public:
		/**
		 * @brief Initializes/registers the fix.
		 */
		static void Install() noexcept;

		/**
		 * @brief Stops combat music.
		 */
		static void Fix();

		/**
		 * @brief Checks conditions to see if combat music is playing when it shouldn't be.
		 * @return If the fix needs to be applied or not.
		 */
		static bool NeedsFix();

		class DeathEventHandler : public RE::BSTEventSink<RE::TESDeathEvent>
		{
		public:
			/**
			 * @brief Singleton getter for class.
			 * @return Class Singleton.
			 */
			[[nodiscard]] static DeathEventHandler* GetSingleton()
			{
				static DeathEventHandler singleton;
				return std::addressof(singleton);
			}

			/**
			 * @brief Checks if the player is in combat when the player kills an enemy. If they are not in combat, it asynchronously stops music after 5 seconds.
			 * @param a_event The event data to evaluate.
			 * @return Event continue statement.
			 */
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent& a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override;

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

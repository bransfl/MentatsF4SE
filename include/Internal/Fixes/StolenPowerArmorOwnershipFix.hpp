#pragma once

namespace Internal::Fixes
{
	class StolenPowerArmorOwnershipFix
		: public Fix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		static void Install() noexcept;

		/**
		 * @brief Class name method.
		 */
		static inline constexpr std::string_view Module::GetModuleName() noexcept
		{
			return "StolenPowerArmorOwnershipFix"sv;
		}

	private:
		/**
		 * @brief Gives the player ownership of the given PowerArmor reference.
		 * @details Setting ownership by executing a console command on the reference is lazy, but it works on both versions.
		 * @param a_powerArmorRef The PowerArmor that will be owned by the player.
		 */
		static void FixOwnership(RE::TESObjectREFR* a_powerArmorRef) noexcept;

		class FurnitureEventHandler : public RE::BSTEventSink<RE::TESFurnitureEvent>
		{
		public:
			/**
			 * @brief Singleton getter for class.
			 * @return Class Singleton.
			 */
			[[nodiscard]] static FurnitureEventHandler* GetSingleton()
			{
				static FurnitureEventHandler singleton;
				return std::addressof(singleton);
			}

			/**
			 * @brief When the player exits power armor, give them ownership of that power armor reference.
			 * @param a_event The event data to evaluate.
			 * @return Event continue statement.
			 */
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent& a_event, RE::BSTEventSource<RE::TESFurnitureEvent>*) override;

			FurnitureEventHandler() = default;
			FurnitureEventHandler(const FurnitureEventHandler&) = delete;
			FurnitureEventHandler(FurnitureEventHandler&&) = delete;
			~FurnitureEventHandler() = default;
			FurnitureEventHandler& operator=(const FurnitureEventHandler&) = delete;
			FurnitureEventHandler& operator=(FurnitureEventHandler&&) = delete;
		};
	};
}

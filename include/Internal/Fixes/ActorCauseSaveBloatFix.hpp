#pragma once

namespace Internal::Fixes
{
	class ActorCauseSaveBloatFix
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
			return "ActorCauseSaveBloatFix"sv;
		}

	private:
		/**
		 * @brief Gathers all non-nullptr references with base formtype kPROJ.
		 * @param a_cell The cell to gather from. If this is null, an empty vector is returned.
		 * @return Vector of found references.
		 */
		static std::vector<RE::TESObjectREFR*> GetProjectilesInCell(RE::TESObjectCELL* a_cell) noexcept;

		class CellAttachDetachEventHandler : public RE::BSTEventSink<RE::CellAttachDetachEvent>
		{
		public:
			/**
			 * @brief Singleton getter for class.
			 * @return Class Singleton.
			 */
			[[nodiscard]] static CellAttachDetachEventHandler* GetSingleton()
			{
				static CellAttachDetachEventHandler singleton;
				return std::addressof(singleton);
			}

			/**
			 * @brief Gathers all non-nullptr references of base formtype kPROJ and removes their ActorCause data.
			 * @param a_event The event data to evaluate.
			 * @return Event continue statement.
			 */
			RE::BSEventNotifyControl ProcessEvent(const RE::CellAttachDetachEvent& a_event, RE::BSTEventSource<RE::CellAttachDetachEvent>*) override;

		private:
			CellAttachDetachEventHandler() = default;
			CellAttachDetachEventHandler(const CellAttachDetachEventHandler&) = delete;
			CellAttachDetachEventHandler(CellAttachDetachEventHandler&&) = delete;
			~CellAttachDetachEventHandler() = default;
			CellAttachDetachEventHandler& operator=(const CellAttachDetachEventHandler&) = delete;
			CellAttachDetachEventHandler& operator=(CellAttachDetachEventHandler&&) = delete;
		};
	};
}

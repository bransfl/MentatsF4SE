#pragma once

namespace Internal::Fixes
{
	class WorkbenchSoundFix
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
			return "WorkbenchSoundFix"sv;
		}

		/**
		 * @brief Stops all workbench SFX annotations on the given reference.
		 * @param a_workbenchUser The reference to silence.
		 * @param a_furniture The furniture reference that a_workbenchUser was using.
		 */
		static void FixWorkbenchSounds(RE::TESObjectREFR* a_workbenchUser, RE::TESFurniture* a_furniture) noexcept;

		// /**
		//  * @brief Checks if the given furniture reference is a workbench.
		//  * @param a_furniture The reference to check.
		//  * @return If the a_furniture is a workbench.
		//  *		   If the reference is null, this returns false.
		//  */
		// static bool IsWorkbench(RE::TESFurniture* a_furniture) noexcept;

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
			 * @brief Checks if the player has exited a workbench, and stops all workbench sound annotations if so.
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

		class ActorCellEventHandler : public RE::BSTEventSink<RE::BGSActorCellEvent>
		{
		public:
			[[nodiscard]] static ActorCellEventHandler* GetSingleton()
			{
				static ActorCellEventHandler singleton;
				return std::addressof(singleton);
			}

			virtual RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent& a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) override
			{
				if (a_event.actor.get().get() && a_event.actor.get().get() != RE::PlayerCharacter::GetSingleton()) {
					return RE::BSEventNotifyControl::kContinue;
				}

				auto ui = RE::UI::GetSingleton();
				if (!ui) {
					return RE::BSEventNotifyControl::kContinue;
				}
				if (ui->GetMenuOpen("ExamineMenu"sv) || ui->GetMenuOpen("CookingMenu"sv)) {
					return RE::BSEventNotifyControl::kContinue;
				}

				FixWorkbenchSounds(a_event.actor.get().get(), nullptr);

				return RE::BSEventNotifyControl::kContinue;
			}

			ActorCellEventHandler() = default;
			ActorCellEventHandler(const ActorCellEventHandler&) = delete;
			ActorCellEventHandler(ActorCellEventHandler&&) = delete;
			~ActorCellEventHandler() = default;
			ActorCellEventHandler& operator=(const ActorCellEventHandler&) = delete;
			ActorCellEventHandler& operator=(ActorCellEventHandler&&) = delete;
		};
	};
}

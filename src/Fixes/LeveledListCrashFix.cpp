#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::LeveledListCrashFix
{
	void Install() noexcept
	{
		// Hooks::ProtectLeveledItems::Install();
		// Hooks::ProtectLeveledActors::Install();
		// Hooks::ProtectLeveledSpells::Install();
	}

	namespace Hooks
	{
		// void DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_problem) {
		// 	logger::info("Caught problematic insertion of form {} to leveled list {}.", a_problem->GetFormEditorID(), form->GetFormEditorID());
		// 	logger::info("EngineFixesF4SE::LeveledListCrashFix -> The form has not been inserted. For ease of review,\nhere are the current contents of the list:\n");
		// 	int i = 1;

		// 	for (auto& entry : a_list->entries) {
		// 		if (!entry.form) {
		// 			logger::info("{}. Null form. This is a problem, do not ignore it.", i);
		// 		}
		// 		else {
		// 			logger::info("{}. {}", i, "_debugEDID(entry.form)");
		// 		}
		// 		++i;
		// 	}
		// }

		// Leveled Items
		void ProtectLeveledItems::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form)
		{
			// if (a_this->numEntries > 254) {
			// 	DebugLeveledList(a_this, a_form);
			// }
			// else {
			_originalCall(a_this, a_list, a_level, a_count, a_form);
			// }
		}

		bool ProtectLeveledItems::Install()
		{
			// F4SE::AllocTrampoline(14);
			// auto& trampoline = F4SE::GetTrampoline();

			// REL::Relocation<std::uintptr_t> target{ REL::ID(55965), 0x56 };
			// if (!(REL::make_pattern<"E8 75 1C 7C FF">().match(target.address()) ||
			// 	REL::make_pattern<"E8 D5 FF 7B FF">().match(target.address()))) {
			// 	logger::info("Failed to validate hook address for ProtectLevItems. Aborting load.");
			// 	return false;
			// }
			logger::info("Installed leveled item patch.");
			//_originalCall = trampoline.write_call<5>(target.address(), &AddForm);
			return true;
		}

		// Leveled Actors
		void ProtectLeveledActors::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form)
		{
			// if (a_this->numEntries > 254) {
			// 	DebugLeveledList(a_this, a_form);
			// }
			// else {
			_originalCall(a_this, a_list, a_level, a_count, a_form);
			// }
		}

		bool ProtectLeveledActors::Install()
		{
			// F4SE::AllocTrampoline(14);
			// auto& trampoline = F4SE::GetTrampoline();

			// REL::Relocation<std::uintptr_t> target{ REL::ID(55954), 0x56 };
			// if (!(REL::make_pattern<"E8 F5 24 7C FF">().match(target.address()) ||
			// 		REL::make_pattern<"E8 55 08 7C FF">().match(target.address()))) {
			// 	logger::info("Failed to validate hook address for ProtectLeveledActors. Aborting load.");
			// 	return false;
			// }

			logger::info("Installed leveled actor patch.");
			//_originalCall = trampoline.write_call<5>(target.address(), &AddForm);
			return true;
		}

		// Leveled Spells
		void ProtectLeveledSpells::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form)
		{
			// if (a_this->numEntries > 254) {
			// 	DebugLeveledList(a_this, a_form);
			// }
			// else {
			_originalCall(a_this, a_list, a_level, a_count, a_form);
			// }
		}

		bool ProtectLeveledSpells::Install()
		{
			// F4SE::AllocTrampoline(14);
			// auto& trampoline = F4SE::GetTrampoline();

			// REL::Relocation<std::uintptr_t> target{ REL::ID(55976), 0x56 };
			// if (!(REL::make_pattern<"E8 65 13 7C FF">().match(target.address()) ||
			// 		REL::make_pattern<"E8 C5 F6 7B FF">().match(target.address()))) {
			// 	logger::info("Failed to validate hook address for ProtectLeveledSpells. Aborting load.");
			// 	return false;
			// }

			logger::info("Installed leveled spell patch.");
			//_originalCall = trampoline.write_call<5>(target.address(), &AddForm);
			return true;
		}
	}

	namespace Sanitizer
	{
		void Sanitize()
		{
			logger::info("LeveledListCrashFix::Sanitizer -> Sanitizing LeveledLists");

			bool foundBadLL = false;

			auto dataHandler = RE::TESDataHandler::GetSingleton();
			auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();

			for (auto* form : formArray) {
				auto* ll = form->As<RE::TESLeveledList>();

				if (!ll) {
					continue;
				}
				std::int8_t numEntries = ll->baseListCount + ll->scriptListCount;
				if (!(numEntries == 0 || numEntries == 255)) {
					continue;
				}

				size_t i = 0;
				// for (auto& llEntry : ll->entries) {
				// 	i++;
				// }
				if (i <= 255) {
					continue;
				}

				logger::info("LeveledListCrashFix::Sanitizer -> LeveledList {} has {} entries", form->GetFormEditorID(), i);
				foundBadLL = true;
			}
			if (foundBadLL) {
				logger::warn("LeveledListCrashFix::Sanitizer -> Warning: At least 1 leveled list has over 255 entries in the plugin record. Check the log at Documents/My Games/Fallout4/F4SE/EngineFixesF4SE.log");
			}
		}
	}
}

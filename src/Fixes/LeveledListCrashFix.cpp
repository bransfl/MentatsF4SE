#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::LeveledListCrashFix
{
	void Install() noexcept
	{
		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("LeveledListCrashFix -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}
		if (std::filesystem::exists("Data/F4SE/Plugins/GLXRM_InjectionBlocker.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'Injection Blocker' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info("LeveledListCrashFix -> InjectionBlocker was installed. Aborting fix.");
			return;
		}

		// Hooks::ProtectLeveledItems::Install();
		// Hooks::ProtectLeveledActors::Install();

		// === example from mgef conditions fix code ===
		// F4SE::AllocTrampoline(8 * 8);
		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
		// if (!REL::Module::IsNG()) {
		// 	// write to OG address
		// 	trampoline.write_branch<5>(ptr_EvaluateConditions_OG.address(), &EvaluateConditions);
		// }
		// else {
		// 	// write to NG address
		// 	// trampoline.write_branch<5>(ptr_EvaluateConditions_NG.address(), &EvaluateConditions);
		// }
	}

	// returns the total amount of leveledlist entries
	int8_t GetListEntriesCount(RE::TESLeveledList* leveledList)
	{
		int8_t ret = leveledList->baseListCount + leveledList->scriptListCount;
		logger::debug("LeveledListCrashFix -> GetListEntriesCount for TODO_NAME_HERE resulted in {}", ret);
		return ret;
	}

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetListEntries(RE::TESLeveledList* leveledList)
	{
		std::vector<RE::TESForm*> ret;
		for (size_t i = 0; i < leveledList->baseListCount; i++) {
			RE::LEVELED_OBJECT& entry = leveledList->leveledLists[i];
			ret.push_back(entry.form);
		}
		for (size_t i = 0; i < leveledList->scriptListCount; i++) {
			RE::LEVELED_OBJECT*& entry = leveledList->scriptAddedLists[i];
			ret.push_back(entry->form);
		}
		return ret;
	}

	namespace Hooks
	{
		// logs any invalid forms within a leveledlist if an error is found
		void DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_problem)
		{
			if (a_problem) {
				// temp for the compiler
			}
			// logger::info("Caught problematic insertion of form {} to leveled list {}.", a_problem->GetFormEditorID(), a_list->GetFormEditorID());
			logger::info("EngineFixesF4SE::LeveledListCrashFix -> The form has not been inserted. For ease of review,\nhere are the current contents of the list:\n");

			int i = 1;
			for (auto* entry : GetListEntries(a_list)) {
				if (!entry) {
					logger::warn("LeveledListCrashFix -> Null form found: {}. This is a problem, do not ignore it.", i);
				}
				++i;
			}
		}

		// Leveled Items
		void ProtectLeveledItems::AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* a_list, unsigned short a_level, unsigned long long a_count, RE::TESForm* a_form)
		{
			if (GetListEntriesCount(a_this) > 254) {
				DebugLeveledList(a_this, a_form);
			}
			else {
				_originalCall(a_this, a_list, a_level, a_count, a_form);
			}
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
			if (GetListEntriesCount(a_this) > 254) {
				DebugLeveledList(a_this, a_form);
			}
			else {
				_originalCall(a_this, a_list, a_level, a_count, a_form);
			}
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
	}

	namespace Sanitizer
	{
		// checks all leveledlists to report if any leveledlists have >255 entries
		void Sanitize()
		{
			logger::info("LeveledListCrashFix::Sanitizer -> Sanitizing LeveledLists");

			bool foundBadLL = false;

			auto dataHandler = RE::TESDataHandler::GetSingleton();
			auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();

			for (auto* form : formArray) {
				auto* leveledList = form->As<RE::TESLeveledList>();

				if (!leveledList) {
					continue;
				}

				int8_t numEntries = GetListEntriesCount(leveledList);
				if (!(numEntries == 0 || numEntries == 255)) {
					continue;
				}

				std::vector<RE::TESForm*> listEntries = GetListEntries(leveledList);
				size_t listEntriesLen = listEntries.size();
				if (listEntriesLen <= 255) {
					continue;
				}

				logger::info("LeveledListCrashFix::Sanitizer -> LeveledList {} has {} entries", form->GetFormEditorID(), listEntriesLen);
				foundBadLL = true;
			}
			if (foundBadLL) {
				logger::warn("LeveledListCrashFix::Sanitizer -> Warning: At least 1 leveled list has over 255 entries in the plugin record. Check the log at Documents/My Games/Fallout4/F4SE/EngineFixesF4SE.log");
			}
		}
	}
}

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
			// logger::info("Caught problematic insertion of form {} to leveled list {}.", _debugEDID(a_problem), _debugEDID(a_list));
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
			F4SE::AllocTrampoline(14);
			auto& trampoline = F4SE::GetTrampoline();

			if (REL::Module::IsNG()) {
				// NG Patch
				logger::info("LeveledListCrashFix -> Game version is NG. Item patch aborted.");
				return false;
			}
			else {
				// OG Patch
				REL::Relocation<uintptr_t> ptr_LeveledItemAddForm_OG{ REL::ID(903957) };
				_originalCall = trampoline.write_branch<5>(ptr_LeveledItemAddForm_OG.address(), &ProtectLeveledItems::AddForm);
			}

			logger::info("LeveledListCrashFix -> Installed leveled item patch.");
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
			F4SE::AllocTrampoline(14);
			auto& trampoline = F4SE::GetTrampoline();

			if (REL::Module::IsNG()) {
				// NG Patch
				logger::info("LeveledListCrashFix -> Game version is NG. Actor patch aborted.");
				return false;
			}
			else {
				// OG Patch
				REL::Relocation<uintptr_t> ptr_LeveledItemAddForm_OG{ REL::ID(1200614) };
				_originalCall = trampoline.write_branch<5>(ptr_LeveledItemAddForm_OG.address(), &ProtectLeveledActors::AddForm);
			}

			logger::info("LeveledListCrashFix -> Installed leveled actor patch.");
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

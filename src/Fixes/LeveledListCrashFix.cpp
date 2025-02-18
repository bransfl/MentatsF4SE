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

		if (REL::Module::IsNG()) {
			// NG Patch - Don't have address for this yet.
			logger::info("LeveledListCrashFix -> Game version was NG. Aborting fix.");
			return;
		}
		else {
			// OG Patch
			F4SE::AllocTrampoline(64);
			F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
			REL::Relocation<uintptr_t> ptr_AddLeveledObject_OG{ REL::ID(1163308) };
			_originalCall = trampoline.write_branch<5>(ptr_AddLeveledObject_OG.address(), &Hook_AddLeveledObject);
		}
		logger::info("LeveledListCrashFix -> Fix applied.");
	}

	RE::LEVELED_OBJECT* Hook_AddLeveledObject(RE::TESLeveledList* a_this, uint16_t a_level, uint16_t a_count, int8_t a_chanceNone, RE::TESForm* a_item, RE::ContainerItemExtra* a_itemExtra)
	{
		if (GetListEntriesCount(a_this) > 254) {
			DebugLeveledList(a_this);
			return nullptr;
		}
		else {
			return _originalCall(a_this, a_level, a_count, a_chanceNone, a_item, a_itemExtra);
		}
	}

	// logs any invalid forms within a leveledlist if an error is found
	void DebugLeveledList(RE::TESLeveledList* a_list)
	{
		// logger::info("Caught problematic insertion of form {} to leveled list {}.", _debugEDID(a_problem), _debugEDID(a_list));
		//logger::info("EngineFixesF4SE::LeveledListCrashFix -> The form has not been inserted. For ease of review,\nhere are the current contents of the list:\n");

		int i = 1;
		for (auto* entry : GetListEntries(a_list)) {
			if (!entry) {
				//logger::warn("LeveledListCrashFix -> Null form found: {}. This is a problem, do not ignore it.", i);
			}
			++i;
		}
	}

	// checks all leveledlists to report if any leveledlists have >255 entries
	void Sanitize()
	{
		logger::info("LeveledListCrashFix::Sanitizer -> Sanitizing LeveledLists");

		bool foundBadLL = false;

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();
		uint32_t listsChecked = 0;

		for (auto* form : formArray) {
			auto* leveledList = form->As<RE::TESLeveledList>();
			listsChecked++;

			if (!leveledList) {
				continue;
			}

			int8_t numEntries = GetListEntriesCount(leveledList);
			if (!(numEntries == 0 || numEntries == 255)) {
				continue;
			}

			// todo - this might be redundant due to the check above?
			size_t listEntriesLen = GetListEntries(leveledList).size();
			if (listEntriesLen <= 255) {
				continue;
			}

			//logger::info("LeveledListCrashFix::Sanitizer -> LeveledList {} has {} entries", form->GetFormEditorID(), listEntriesLen);
			foundBadLL = true;
		}
		if (foundBadLL) {
			logger::warn("LeveledListCrashFix::Sanitizer -> Warning: At least 1 leveled list has over 255 entries in the plugin record. Check the log at Documents/My Games/Fallout4/F4SE/EngineFixesF4SE.log");
		}
		logger::info("LeveledListCrashFix::Sanitizer -> listsChecked={}", listsChecked);
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
}

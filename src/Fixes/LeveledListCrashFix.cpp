#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"

#pragma warning(disable : 4100) // for RegisterHookOG

// qudix's RE:
// TESLeveledList::AddScriptAddedLeveledObject
// REL::Relocation target{ REL::ID(2193269), 0x6D };
// OriginalFunction = target.write_call<5>(Hook_AddLeveledObject);

namespace Internal::Fixes::LeveledListCrashFix
{
	// function signature definition
	typedef RE::LEVELED_OBJECT*(AddLeveledObjectSig)(RE::TESLeveledList* a_this, uint16_t a_level, uint16_t a_count, int8_t chanceNone, RE::TESForm* a_item, RE::ContainerItemExtra* a_itemExtra);
	REL::Relocation<AddLeveledObjectSig> OriginalFunction;

	// installs the fix
	void Install() noexcept
	{
		logger::info("Fix installing: LeveledListCrashFix.");

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in ini file.");
			return;
		}
		if (std::filesystem::exists("Data/F4SE/Plugins/GLXRM_InjectionBlocker.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'Injection Blocker' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Mod was installed: GLXRM_InjectionBlocker.dll.");
			return;
		}

		if (REL::Module::IsNG()) {
			// auto& trampoline = F4SE::GetTrampoline();
			// trampoline.create(64); // idk how big this one needs to be
			// RegisterHookNG(trampoline);
		}
		else {
			// OG Patch
			// auto& trampoline = F4SE::GetTrampoline();
			// trampoline.create(64); // idk how big this one needs to be
			// RegisterHookOG(trampoline);
		}

		logger::info("Fix installed: LeveledListCrashFix");
	}

	// the hook
	RE::LEVELED_OBJECT* Hook_AddLeveledObject(RE::TESLeveledList* a_this, uint16_t a_level, uint16_t a_count, int8_t chanceNone, RE::TESForm* a_item, RE::ContainerItemExtra* a_itemExtra)
	{
		if (GetListEntriesCount(a_this) > 254) {
			DebugLeveledList(a_this);
			return nullptr; // pray they added handling for this
		}
		else {
			// normal process
			return OriginalFunction(a_this, a_level, a_count, chanceNone, a_item, a_itemExtra);
		}
	}

	// the hooker
	void RegisterHookOG(F4SE::Trampoline& trampoline)
	{
		// REL::Relocation<AddLeveledObjectSig> callLocation{ REL::ID(1163308) }; // TODO idk what offset this might need
		// OriginalFunction = trampoline.write_call<5>(callLocation.address(), &Hook_AddLeveledObject);
	}

	// the hooker
	void RegisterHookNG(F4SE::Trampoline& trampoline)
	{
		REL::Relocation target{ REL::ID(2193269), 0x6D };
		OriginalFunction = target.write_call<5>(Hook_AddLeveledObject);
	}

	// logs any invalid forms within a leveledlist if an error is found
	void DebugLeveledList(RE::TESLeveledList* a_list)
	{
		int i = 1;
		for (auto* entry : GetListEntries(a_list)) {
			if (!entry) {
				logger::warn("LeveledListCrashFix -> Null form found: {}. This is a problem, do not ignore it.", i);
			}
			++i;
		}
	}

	// checks all leveledlists to report if any leveledlists have >255 entries
	void Sanitize()
	{
		logger::info("LeveledListCrashFix::Sanitizer -> Sanitizing LeveledLists");

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();
		uint32_t listsChecked = 0;
		bool foundBadLL = false;

		for (auto* form : formArray) {
			listsChecked++;
			auto* leveledList = form->As<RE::TESLeveledList>();
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

			// logger::info("LeveledListCrashFix::Sanitizer -> LeveledList {} has {} entries", form->GetFormEditorID(), listEntriesLen);
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

#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"
#include "detourxs/detourxs.h"

// qudix's RE:
// TESLeveledList::AddScriptAddedLeveledObject
// REL::Relocation target{ REL::ID(2193269), 0x6D };
// OriginalFunction = target.write_call<5>(Hook_AddLeveledObject);

namespace Internal::Fixes::LeveledListCrashFix
{
	// typedefs
	typedef void(mem_LeveledItemAddFormSig)(RE::BSScript::IVirtualMachine*, uint32_t, RE::TESLevItem*, RE::TESForm*, uint32_t, uint32_t);
	REL::Relocation<mem_LeveledItemAddFormSig> OriginalFunction_LeveledItemAddForm;
	DetourXS itemHook;

	typedef void(mem_LeveledActorAddFormSig)(RE::BSScript::IVirtualMachine*, uint32_t, RE::TESLevCharacter*, RE::TESForm*, uint32_t);
	REL::Relocation<mem_LeveledActorAddFormSig> OriginalFunction_LeveledActorAddForm;
	DetourXS actorHook;

	// installs the fix
	void Install() noexcept
	{
		logger::info("Fix installing: LeveledListCrashFix."sv);

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in ini file."sv);
			return;
		}
		if (std::filesystem::exists("Data/F4SE/Plugins/GLXRM_InjectionBlocker.dll"sv)) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'Injection Blocker' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::warn("Fix aborted: LeveledListCrashFix. Reason: Mod was installed: GLXRM_InjectionBlocker.dll."sv);
			return;
		}

		if (REL::Module::IsNG()) {
			// NG Patch
			logger::info("Fix aborted: MagicEffectConditions. Reason: Game version was NG."sv);
			return;
		}
		else {
			// OG Patch
			logger::info("LeveledListCrashFix -> Hooks installing..."sv);

			// item
			REL::Relocation<mem_LeveledItemAddFormSig> itemFuncLocation{ REL::ID(903957) };
			if (itemHook.Create(reinterpret_cast<LPVOID>(itemFuncLocation.address()), &Hook_LeveledItemAddForm)) {
				OriginalFunction_LeveledItemAddForm = reinterpret_cast<std::uintptr_t>(itemHook.GetTrampoline());
				logger::info("LeveledListCrashFix -> Successfully created Item hook"sv);
			}
			else {
				logger::warn("LeveledListCrashFix -> Failed to create Item hook"sv);
			}

			// actor
			REL::Relocation<mem_LeveledActorAddFormSig> actorFuncLocation{ REL::ID(1200614) };
			if (actorHook.Create(reinterpret_cast<LPVOID>(actorFuncLocation.address()), &Hook_LeveledActorAddForm)) {
				OriginalFunction_LeveledActorAddForm = reinterpret_cast<std::uintptr_t>(actorHook.GetTrampoline());
				logger::info("LeveledListCrashFix -> Successfully created Actor hook"sv);
			}
			else {
				logger::warn("LeveledListCrashFix -> Failed to create Actor hook"sv);
			}

			logger::info("LeveledListCrashFix -> Hooks installed."sv);
		}

		logger::info("Fix installed: LeveledListCrashFix"sv);
	}

	// the hooks
	void Hook_LeveledItemAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevItem* a_leveledItemList, RE::TESForm* a_formToAdd, uint32_t a_level, uint32_t a_count)
	{
		logger::info("LeveledListCrashFix -> Hook_LeveledItemAddForm ran for {}: numEntries={}", a_leveledItemList->As<RE::TESForm>()->GetFormEditorID(), GetNumEntries(a_leveledItemList->As<RE::TESLeveledList>()));
		if (GetNumEntries(a_leveledItemList->As<RE::TESLeveledList>()) > 254) {
			DebugLeveledList(a_leveledItemList->As<RE::TESLeveledList>());
			logger::warn("LeveledListCrashFix -> Full item leveledlist found: {}", a_leveledItemList->GetFormEditorID());
			a_vm->PostError("LeveledListCrashFix -> Full item leveledlist found. Check EngineFixesF4SE.log for more information.", a_stackID, RE::BSScript::ErrorLogger::Severity::kError);
			return;
		}
		else {
			OriginalFunction_LeveledItemAddForm(a_vm, a_stackID, a_leveledItemList, a_formToAdd, a_level, a_count);
		}
	}

	void Hook_LeveledActorAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevCharacter* a_leveledCharacterList, RE::TESForm* a_formToAdd, uint32_t a_level)
	{
		logger::info("LeveledListCrashFix -> Hook_LeveledActorAddForm ran for {}: numEntries={}", a_leveledCharacterList->As<RE::TESForm>()->GetFormEditorID(), GetNumEntries(a_leveledCharacterList->As<RE::TESLeveledList>()));
		if (GetNumEntries(a_leveledCharacterList->As<RE::TESLeveledList>()) > 254) {
			DebugLeveledList(a_leveledCharacterList->As<RE::TESLeveledList>());
			logger::warn("LeveledListCrashFix -> Full actor leveledlist found: {}", a_leveledCharacterList->GetFormEditorID());
			a_vm->PostError("LeveledListCrashFix -> Full actor leveledlist found. Check EngineFixesF4SE.log for more information.", a_stackID, RE::BSScript::ErrorLogger::Severity::kError);
			return;
		}
		else {
			OriginalFunction_LeveledActorAddForm(a_vm, a_stackID, a_leveledCharacterList, a_formToAdd, a_level);
		}
	}

	// logs any invalid forms within a leveledlist if an error is found
	void DebugLeveledList(RE::TESLeveledList* a_list)
	{
		logger::info("LeveledListCrashFix -> DebugLeveledList started..."sv);

		int i = 1;
		for (auto* entry : GetEntries(a_list)) {
			if (!entry) {
				logger::warn("LeveledListCrashFix -> Null form found: {} at: {}. This is a problem, do not ignore it.", entry->GetFormEditorID(), i);
			}
			++i;
		}

		logger::info("LeveledListCrashFix -> DebugLeveledList finished."sv);
	}

	// checks all leveledlists to report if any leveledlists have >255 entries
	void Sanitize()
	{
		logger::info("LeveledListCrashFix -> Sanitizing LeveledLists"sv);

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
			int8_t numEntries = GetNumEntries(leveledList);
			if (!(numEntries == 0 || numEntries == 255)) {
				continue;
			}
			if (numEntries <= 255) {
				continue;
			}

			logger::info("LeveledListCrashFix::Sanitizer -> LeveledList {} has {} entries", form->GetFormEditorID(), numEntries);
			foundBadLL = true;
		}
		if (foundBadLL) {
			logger::warn("LeveledListCrashFix::Sanitizer -> Warning: At least 1 leveled list has over 255 entries in the plugin record. Check the log at Documents/My Games/Fallout4/F4SE/EngineFixesF4SE.log"sv);
		}
		logger::info("LeveledListCrashFix::Sanitizer -> listsChecked={}", listsChecked);
	}

	// returns the total amount of leveledlist entries
	int8_t GetNumEntries(RE::TESLeveledList* leveledList)
	{
		int8_t ret = leveledList->baseListCount + leveledList->scriptListCount;
		logger::debug("LeveledListCrashFix -> GetNumEntries for TODO_NAME_HERE resulted in {}", ret);
		return ret;
	}

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetEntries(RE::TESLeveledList* leveledList)
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

#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
// #include "detourxs/detourxs.h"

// qudix's RE:
// TESLeveledList::AddScriptAddedLeveledObject
// REL::Relocation target{ REL::ID(2193269), 0x6D };
// OriginalFunction = target.write_call<5>(Hook_AddLeveledObject);

namespace Internal::Fixes
{
	// typedefs for function signature and original function declaration
	typedef void(mem_AddScriptAddedLeveledObjectSig)(RE::TESLeveledList*, RE::TESForm*, uint16_t, uint16_t, RE::TESForm*);
	REL::Relocation<mem_AddScriptAddedLeveledObjectSig> OriginalFunction_AddScriptAddedLeveledObject;

	// typedef void(mem_LeveledItemAddFormSig)(RE::BSScript::IVirtualMachine*, uint32_t, RE::TESLevItem*, RE::TESForm*, uint32_t, uint32_t);
	// REL::Relocation<mem_LeveledItemAddFormSig> OriginalFunction_LeveledItemAddForm;
	// DetourXS itemHook;

	// typedef void(mem_LeveledActorAddFormSig)(RE::BSScript::IVirtualMachine*, uint32_t, RE::TESLevCharacter*, RE::TESForm*, uint32_t);
	// REL::Relocation<mem_LeveledActorAddFormSig> OriginalFunction_LeveledActorAddForm;
	// DetourXS actorHook;

	void LeveledListCrashFix::Install() noexcept
	{
		logger::info("Fix installing: LeveledListCrashFix."sv);

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in ini file."sv);
			return;
		}
		if (REX::W32::GetModuleHandleW(L"GLXRM_InjectionBlocker.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Fix aborted: LeveledListCrashFix. Reason: Mod 'Injection Blocker' was detected. This fix is not necessary with this mod installed.\n");
			logger::warn("Fix aborted: LeveledListCrashFix. Reason: Mod was installed: GLXRM_InjectionBlocker.dll."sv);
			return;
		}

		F4SE::AllocTrampoline(16);

		logger::info("LeveledListCrashFix -> Hooks installing..."sv);
		if (REL::Module::IsNG()) {
			// NG Patch
			REL::Relocation target_NG{ REL::ID(2193269), 0x6D };
			OriginalFunction_AddScriptAddedLeveledObject = target_NG.write_call<5>(Hook_AddScriptAddedLeveledObject);
		}
		else {
			// OG Patch
			REL::Relocation target_OG{ REL::ID(860553), 0x6C };
			OriginalFunction_AddScriptAddedLeveledObject = target_OG.write_call<5>(Hook_AddScriptAddedLeveledObject);
		}
		logger::info("LeveledListCrashFix -> Hooks installed."sv);

		logger::info("Fix installed: LeveledListCrashFix"sv);
	}

	void LeveledListCrashFix::Hook_AddScriptAddedLeveledObject(RE::TESLeveledList* a_this, RE::TESForm* a_owner, uint16_t a_level, uint16_t a_count, RE::TESForm* a_form)
	{
		if (Utility::GetNumEntries(a_this) > 254) {
			LeveledListCrashFix::DebugLeveledList(a_this, a_form);
			return;
		}
		else {
			// logger::info("ll crash fix orig func ran (temp)"sv);
			return OriginalFunction_AddScriptAddedLeveledObject(a_this, a_owner, a_level, a_count, a_form);
		}
	}

	// if (Utility::GetNumEntries(a_leveledCharacterList->As<RE::TESLeveledList>()) > 254) {
	// 	LeveledListCrashFix::DebugLeveledListActor(a_leveledCharacterList, a_formToAdd);
	// 	logger::warn(FMT_STRING("LeveledListCrashFix -> Full actor leveledlist found. FormID: {:08X}, EditorID: {}"), a_leveledCharacterList->GetFormID(), a_leveledCharacterList->GetFormEditorID());
	// 	a_vm->PostError("LeveledListCrashFix -> Full actor leveledlist found. Check EngineFixesF4SE.log for more information.", a_stackID, RE::BSScript::ErrorLogger::Severity::kError);
	// 	return;
	// }
	// else {
	// 	OriginalFunction_LeveledActorAddForm(a_vm, a_stackID, a_leveledCharacterList, a_formToAdd, a_level);
	// }

	void LeveledListCrashFix::DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_form)
	{
		// logger::info(FMT_STRING("Caught problematic insertion of (FormID: {:08X}, EditorID: {}) into item LeveledList (FormID: {:08X}, EditorID: {})"),
		// 	a_form->GetFormID(), a_form->GetFormEditorID(), a_list->GetFormID(), a_list->GetFormEditorID());
		// logger::info("The form has not been inserted. For ease of review, here are the current contents of the list:\n"sv);

		if (!a_form) {
			//
		}
		int i = 1;
		for (auto& entry : LeveledListCrashFix::GetEntries(a_list)) {
			if (!entry->GetFormID()) {
				logger::warn(FMT_STRING("LeveledListCrashFix -> Index: {} has NULL form. This is a problem, do not ignore it."), i);
			}
			else {
				logger::warn(FMT_STRING("LeveledListCrashFix -> Index: {} at has form: {}."), i, entry->GetFormEditorID());
			}
			i++;
		}
		logger::info("---------------------------------------------"sv);
	}

	std::vector<RE::TESForm*> LeveledListCrashFix::GetEntries(RE::TESLeveledList* leveledList)
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

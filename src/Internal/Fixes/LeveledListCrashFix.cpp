#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
// #include "detourxs/detourxs.h"

namespace Internal::Fixes
{
	// typedefs for function signature and original function declaration
	typedef void(mem_AddScriptAddedLeveledObjectSig)(RE::TESLeveledList*, RE::TESForm*, uint16_t, uint16_t, RE::TESForm*);
	REL::Relocation<mem_AddScriptAddedLeveledObjectSig> OriginalFunction_AddScriptAddedLeveledObject;

	void LeveledListCrashFix::Install() noexcept
	{
		logger::info(FMT_STRING("Fix installing: LeveledListCrashFix."sv));

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in config file."sv));
			return;
		}
		if (REX::W32::GetModuleHandleW(L"GLXRM_InjectionBlocker.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Fix aborted: LeveledListCrashFix. Reason: Mod 'Injection Blocker' was detected. This fix is not necessary with this mod installed.\n");
			logger::warn(FMT_STRING("Fix aborted: LeveledListCrashFix. Reason: Mod was installed: GLXRM_InjectionBlocker.dll."sv));
			return;
		}

		F4SE::AllocTrampoline(16);

		logger::info(FMT_STRING("LeveledListCrashFix -> Hooks installing..."sv));
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
		logger::info(FMT_STRING("LeveledListCrashFix -> Hooks installed."sv));

		logger::info(FMT_STRING("Fix installed: LeveledListCrashFix"sv));
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

	void LeveledListCrashFix::DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_form)
	{
		// logger::info(FMT_STRING("Caught problematic insertion of (FormID: {:08X}, EditorID: {}) into item LeveledList (FormID: {:08X}, EditorID: {})"),
		// 	a_form->GetFormID(), a_form->GetFormEditorID(), a_list->GetFormID(), a_list->GetFormEditorID());
		// logger::info("The form has not been inserted. For ease of review, here are the current contents of the list:\n"sv);

		if (!a_form) {
			// compiler moment
		}
		int i = 1;
		for (auto& entry : LeveledListCrashFix::GetEntries(a_list)) {
			if (!entry->GetFormID()) {
				logger::warn(FMT_STRING("LeveledListCrashFix -> Index: {} has NULL form. This is a problem, do not ignore it."), i);
			}
			else {
				logger::warn(FMT_STRING("LeveledListCrashFix -> Index: {} at has FormID: {:08X}, EditorID: {}."), i, entry->GetFormID(), entry->GetFormEditorID());
			}
			i++;
		}
		logger::info(FMT_STRING("---------------------------------------------"sv));
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

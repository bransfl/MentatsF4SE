#include "Internal/Fixes/LeveledListCrashFix.hpp"
// #include "detourxs/detourxs.h"

namespace Internal::Fixes
{
	typedef void(Signature_AddScriptAddedLeveledObject)(RE::TESLeveledList*, RE::TESForm*, uint16_t, uint16_t, RE::TESForm*);
	REL::Relocation<Signature_AddScriptAddedLeveledObject> OriginalFunction_AddScriptAddedLeveledObject;

	void LeveledListCrashFix::Install() noexcept
	{
		logger::info("Fix installing: LeveledListCrashFix."sv);

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in config."sv);
			return;
		}

		if (REX::W32::GetModuleHandleW(L"GLXRM_InjectionBlocker.dll")) {
			logger::warn("Fix aborted: LeveledListCrashFix. Reason: Mod was installed: GLXRM_InjectionBlocker.dll."sv);
			return;
		}

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

		logger::info("Fix installed: LeveledListCrashFix."sv);
	}

	void LeveledListCrashFix::Hook_AddScriptAddedLeveledObject(RE::TESLeveledList* a_this, RE::TESForm* a_owner, uint16_t a_level, uint16_t a_count, RE::TESForm* a_form)
	{
		if (Utility::GetNumEntries(a_this) > 254) {
			LeveledListCrashFix::DebugLeveledList(a_this, a_form);
			return;
		}
		else {
			return OriginalFunction_AddScriptAddedLeveledObject(a_this, a_owner, a_level, a_count, a_form);
		}
	}

	void LeveledListCrashFix::DebugLeveledList(RE::TESLeveledList* a_list, RE::TESForm* a_form)
	{
		// logger::info("Caught problematic insertion of (FormID: {:08X}, EditorID: {}) into item LeveledList (FormID: {:08X}, EditorID: {})"),
		// 	a_form->GetFormID(), a_form->GetFormEditorID(), a_list->GetFormID(), a_list->GetFormEditorID());
		// logger::info("The form has not been inserted. For ease of review, here are the current contents of the list:\n"sv);

		logger::warn("LeveledListCrashFix -> DebugLeveledList -> Insertion of Form a_form: {} was prevented."sv,
			Utility::GetFormInfo(a_form));

		int i = 1;
		for (auto& entry : LeveledListCrashFix::GetEntries(a_list)) {
			if (!entry || !entry->GetFormID()) {
				logger::warn("\tLeveledListCrashFix -> [WARNING] - Form at index: {} is nullptr. This is a problem, do not ignore it."sv, i);
			}
			else {
				logger::warn("\tLeveledListCrashFix -> Form at index: {} is {}."sv, i, Utility::GetFormInfo(entry));
			}
			i++;
		}
		logger::info("---------------------------------------------"sv);
	}

	std::vector<RE::TESForm*> LeveledListCrashFix::GetEntries(RE::TESLeveledList* a_leveledList)
	{
		std::vector<RE::TESForm*> ret;

		for (size_t i = 0; i < a_leveledList->baseListCount; i++) {
			RE::LEVELED_OBJECT& entry = a_leveledList->leveledLists[i];
			ret.push_back(entry.form);
		}

		for (size_t i = 0; i < a_leveledList->scriptListCount; i++) {
			RE::LEVELED_OBJECT*& entry = a_leveledList->scriptAddedLists[i];
			ret.push_back(entry->form);
		}

		return ret;
	}
}

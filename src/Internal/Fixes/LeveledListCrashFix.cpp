#include "Internal/Fixes/LeveledListCrashFix.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"
#include "detourxs/detourxs.h"

// qudix's RE:
// TESLeveledList::AddScriptAddedLeveledObject
// REL::Relocation target{ REL::ID(2193269), 0x6D };
// OriginalFunction = target.write_call<5>(Hook_AddLeveledObject);

namespace Internal::Fixes
{
	// typedefs for function signature and original function declaration
	typedef void(mem_LeveledItemAddFormSig)(RE::BSScript::IVirtualMachine*, uint32_t, RE::TESLevItem*, RE::TESForm*, uint32_t, uint32_t);
	REL::Relocation<mem_LeveledItemAddFormSig> OriginalFunction_LeveledItemAddForm;
	DetourXS itemHook;

	typedef void(mem_LeveledActorAddFormSig)(RE::BSScript::IVirtualMachine*, uint32_t, RE::TESLevCharacter*, RE::TESForm*, uint32_t);
	REL::Relocation<mem_LeveledActorAddFormSig> OriginalFunction_LeveledActorAddForm;
	DetourXS actorHook;

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

		if (REL::Module::IsNG()) {
			// NG Patch
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Game version was NG."sv);
			return;
		}
		else {
			// OG Patch
			logger::info("LeveledListCrashFix -> Hooks installing..."sv);

			// item
			REL::Relocation<mem_LeveledItemAddFormSig> targetItem_OG{ REL::ID(903957) };
			if (itemHook.Create(reinterpret_cast<LPVOID>(targetItem_OG.address()), &Hook_LeveledItemAddForm)) {
				OriginalFunction_LeveledItemAddForm = reinterpret_cast<std::uintptr_t>(itemHook.GetTrampoline());
				logger::info("LeveledListCrashFix -> Successfully created Item hook."sv);
			}
			else {
				logger::warn("LeveledListCrashFix -> Failed to create Item hook."sv);
			}

			// actor
			REL::Relocation<mem_LeveledActorAddFormSig> targetActor_OG{ REL::ID(1200614) };
			if (actorHook.Create(reinterpret_cast<LPVOID>(targetActor_OG.address()), &Hook_LeveledActorAddForm)) {
				OriginalFunction_LeveledActorAddForm = reinterpret_cast<std::uintptr_t>(actorHook.GetTrampoline());
				logger::info("LeveledListCrashFix -> Successfully created Actor hook."sv);
			}
			else {
				logger::warn("LeveledListCrashFix -> Failed to create Actor hook."sv);
			}

			logger::info("LeveledListCrashFix -> Hooks installed."sv);
		}

		logger::info("Fix installed: LeveledListCrashFix"sv);
	}

	void Hook_LeveledItemAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevItem* a_leveledItemList, RE::TESForm* a_formToAdd, uint32_t a_level, uint32_t a_count)
	{
		if (Utility::GetNumEntries(a_leveledItemList->As<RE::TESLeveledList>()) > 254) {
			LeveledListCrashFix::DebugLeveledListItem(a_leveledItemList, a_formToAdd);
			logger::warn(FMT_STRING("LeveledListCrashFix -> Full item leveledlist found. FormID: {:08X}, EditorID: {}"), a_leveledItemList->GetFormID(), a_leveledItemList->GetFormEditorID());
			a_vm->PostError("LeveledListCrashFix -> Full item leveledlist found. Check EngineFixesF4SE.log for more information.", a_stackID, RE::BSScript::ErrorLogger::Severity::kError);
			return;
		}
		else {
			OriginalFunction_LeveledItemAddForm(a_vm, a_stackID, a_leveledItemList, a_formToAdd, a_level, a_count);
		}
	}

	void Hook_LeveledActorAddForm(RE::BSScript::IVirtualMachine* a_vm, std::uint32_t a_stackID, RE::TESLevCharacter* a_leveledCharacterList, RE::TESForm* a_formToAdd, uint32_t a_level)
	{
		if (Utility::GetNumEntries(a_leveledCharacterList->As<RE::TESLeveledList>()) > 254) {
			LeveledListCrashFix::DebugLeveledListActor(a_leveledCharacterList, a_formToAdd);
			logger::warn(FMT_STRING("LeveledListCrashFix -> Full actor leveledlist found. FormID: {:08X}, EditorID: {}"), a_leveledCharacterList->GetFormID(), a_leveledCharacterList->GetFormEditorID());
			a_vm->PostError("LeveledListCrashFix -> Full actor leveledlist found. Check EngineFixesF4SE.log for more information.", a_stackID, RE::BSScript::ErrorLogger::Severity::kError);
			return;
		}
		else {
			OriginalFunction_LeveledActorAddForm(a_vm, a_stackID, a_leveledCharacterList, a_formToAdd, a_level);
		}
	}

	void LeveledListCrashFix::DebugLeveledListItem(RE::TESLevItem* a_list, RE::TESForm* a_form)
	{
		logger::info(FMT_STRING("Caught problematic insertion of (FormID: {:08X}, EditorID: {}) into item LeveledList (FormID: {:08X}, EditorID: {})"),
			a_form->GetFormID(), a_form->GetFormEditorID(), a_list->GetFormID(), a_list->GetFormEditorID());
		logger::info("The form has not been inserted. For ease of review, here are the current contents of the list:\n");

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

	void LeveledListCrashFix::DebugLeveledListActor(RE::TESLevCharacter* a_list, RE::TESForm* a_form)
	{
		logger::info(FMT_STRING("Caught problematic insertion of (FormID: {:08X}, EditorID: {}) into actor LeveledList (FormID: {:08X}, EditorID: {})"),
			a_form->GetFormID(), a_form->GetFormEditorID(), a_list->GetFormID(), a_list->GetFormEditorID());
		logger::info("The form has not been inserted. For ease of review, here are the current contents of the list:\n");

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

#include "Internal/Utility.hpp"

namespace Internal
{
	void Utility::ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent)
	{
		auto* log = RE::ConsoleLog::GetSingleton();
		auto compiler = RE::ScriptCompiler();

		auto* scriptFactory = RE::ConcreteFormFactory<RE::Script>::GetFormFactory();
		auto* script = scriptFactory->Create();
		auto buffer = log->buffer;

		script->SetText(a_command);
		script->CompileAndRun(&compiler, RE::COMPILER_NAME::kSystemWindow, a_targetRef);

		if (!script->header.isCompiled) {
			logger::info("Utility -> ExecuteCommand: Failed to compile command: {}"sv, a_command);
		}

		if (a_silent == true) {
			log->buffer = std::move(buffer);
		}

		delete script;
	}

	std::string_view Utility::GetModName(RE::TESForm* a_form, bool a_lastModified)
	{
		const auto index = a_lastModified ? -1 : 0;
		const auto* file = a_form->GetFile(index);
		if (file) {
			return file->filename;
		}
		else {
			return "MODNAME_NOT_FOUND"sv;
		}
	}

	int32_t Utility::GetNumEntries(RE::TESLeveledList* a_leveledList)
	{
		if (!a_leveledList) {
			logger::warn("Utility -> GetNumEntries -> a_leveledList was null.sv");
			return -1;
		}

		return a_leveledList->baseListCount + a_leveledList->scriptListCount;
	}

	std::string_view Utility::GetFormInfo(RE::TESForm* a_form)
	{
		if (!a_form) {
			return "(FormID: nullptr, EditorID: nullptr)"sv;
		}

		RE::TESFormID formID = a_form->GetFormID();
		std::string_view editorID = a_form->GetFormEditorID();
		return "(FormID: {:08X}, EditorID: {})", formID, editorID;
	}
}

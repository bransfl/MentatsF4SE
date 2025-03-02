#include "Internal/Utility/Utility.hpp"

namespace Internal::Utility
{
	void ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent)
	{
		auto* log = RE::ConsoleLog::GetSingleton();
		auto compiler = RE::ScriptCompiler();

		auto* scriptFactory = RE::ConcreteFormFactory<RE::Script>::GetFormFactory();
		auto* script = scriptFactory->Create();
		auto buffer = log->buffer;

		script->SetText(a_command);
		script->CompileAndRun(&compiler, RE::COMPILER_NAME::kSystemWindow, a_targetRef);

		if (!script->header.isCompiled) {
			logger::info(FMT_STRING("Utility -> ExecuteCommand: Failed to compile command: {}"), a_command);
		}

		if (a_silent == true) {
			log->buffer = std::move(buffer);
		}
		delete script;
	}

	std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified)
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

	int8_t GetNumEntries(RE::TESLeveledList* leveledList)
	{
		if (!leveledList) {
			return -1;
		}
		return leveledList->baseListCount + leveledList->scriptListCount;
	}
}

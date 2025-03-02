#include "Internal/Utility/Utility.hpp"

namespace Internal::Utility
{
	// runs a console command
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
			logger::info("Internal::Utility::Console::ExecuteCommand: Failed to compile command \"{}\""sv, a_command);
		}

		if (a_silent == true) {
			log->buffer = std::move(buffer);
		}
		delete script;
	}

	// returns the name of the mod that a_form is from
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

	// returns the total amount of leveledlist entries
	int8_t GetNumEntries(RE::TESLeveledList* leveledList)
	{
		if (!leveledList) {
			return -1;
		}
		return leveledList->baseListCount + leveledList->scriptListCount;
	}
}

#include "Internal/Utility/Utility.hpp"

namespace Internal::Utility
{
	namespace Console
	{
		// runs a console command
		void ExecuteCommand(std::string_view a_command, bool a_silent)
		{
			auto* log = RE::ConsoleLog::GetSingleton();
			auto compiler = RE::ScriptCompiler();

			auto* scriptFactory = RE::ConcreteFormFactory<RE::Script>::GetFormFactory();
			auto* script = scriptFactory->Create();
			auto buffer = log->buffer;

			script->SetText(a_command);
			// todo check if nullptr is preventing it from running
			script->CompileAndRun(&compiler, RE::COMPILER_NAME::kSystemWindow, nullptr);

			if (!script->header.isCompiled) {
				//logger:info("Internal::Utility::Console::ExecuteCommand: Failed to compile command \"{}\""sv, a_command);
			}

			if (a_silent == true) {
				log->buffer = std::move(buffer);
			}
			delete script;
		}
	}
}

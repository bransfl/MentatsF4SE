#include "Internal/Utility/Utility.hpp"

namespace Internal::Utility
{
	// inline void sendConsoleCommand(const std::string& a_command)
	// {
	// 	const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
	// 	if (const auto script = scriptFactory ? scriptFactory->Create() : nullptr) {
	// 		const auto selectedRef = RE::Console::GetSelectedRef();
	// 		script->SetCommand(a_command);
	// 		script->CompileAndRun(selectedRef.get());
	// 		delete script;
	// 	}
	namespace Console
	{
		// runs a console command
		bool ExecuteCommand(const std::string& a_command)
		{
			auto compiler = RE::ScriptCompiler();
			auto* scriptFactory = RE::ConcreteFormFactory<RE::Script>::GetFormFactory();
			if (const auto script = scriptFactory ? scriptFactory->Create() : nullptr) {
				const auto menu = RE::UI::GetSingleton()->GetMenu<RE::Console>();
				if (!menu)
					return false;
				const auto refHandle = menu->GetCurrentPickREFR();
				const auto selectedRef = refHandle ? refHandle.get().get() : nullptr;
				script->SetText(a_command);
				script->CompileAndRun(&compiler, RE::COMPILER_NAME::kSystemWindow, selectedRef);
				delete script;
			}
			return true;
		}
	}
}

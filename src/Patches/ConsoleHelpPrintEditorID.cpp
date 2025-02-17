#include "Internal/Config/Config.hpp"

// prints item editorids in the console when 'help "item" 4 TYPE' is used
namespace Internal::Patches::ConsoleHelpPrintEditorID
{
	void Install() noexcept
	{
		if (!Config::bConsoleHelpPrintEditorID.GetValue()) {
			logger::info("ConsoleHelpPrintEditorID -> Patch was disabled in the ini file. Patch aborted.");
			return;
		}
	}
}

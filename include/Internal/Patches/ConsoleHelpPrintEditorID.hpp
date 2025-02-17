#pragma once

// prints item editorids in the console when 'help "item" 4 TYPE' is used
namespace Internal::Patches::ConsoleHelpPrintEditorID
{
	void Install() noexcept;
}
#include "Internal/Patches/Installation.hpp"
#include "Internal/Patches/ConsoleHelpPrintEditorID.hpp"
#include "Internal/Patches/LoadEditorIDs.hpp"

namespace Internal::Patches
{
	void Install() noexcept
	{
		logger::info("Installing patches..."sv);

		ConsoleHelpPrintEditorID::Install();
		LoadEditorIDs::Install();

		logger::info("Patches installed."sv);
	}
}

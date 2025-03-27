#include "Internal/Warnings/Installation.hpp"

namespace Internal::Warnings
{
	void Installation::Install() noexcept
	{
		logger::info("Warnings installing..."sv);

		// DupeAddonNodesWarning is handled in Messaging

		// LeveledListEntryCountWarning is handled in Messaging

		logger::info("Warnings installed."sv);
	}
}

#pragma once

namespace Internal::Warnings
{
	class LeveledListEntryCountWarning
	{
	public:
		// checks all leveledlists to report if they have over 255 entries
		static void CheckLeveledLists();
	};
}

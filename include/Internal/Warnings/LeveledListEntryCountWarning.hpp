#pragma once

namespace Internal::Warnings::LeveledListEntryCountWarning
{
	void CheckLeveledLists();

	int8_t GetNumEntries(RE::TESLeveledList* leveledList);
}

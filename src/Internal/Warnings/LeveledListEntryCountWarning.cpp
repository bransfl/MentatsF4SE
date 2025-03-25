#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility.hpp"

namespace Internal::Warnings
{
	void LeveledListEntryCountWarning::Install()
	{
		logger::info("LeveledListEntryCountWarning -> Warning installing..."sv);

		if (!Config::bLeveledListEntryCountWarning.GetValue()) {
			logger::info("Warning aborted: LeveledListEntryCountWarning. Reason: Process was disabled in toml file."sv);
			return;
		}

		CheckLeveledLists();

		logger::info("LeveledListEntryCountWarning -> Warning installed."sv);
	}

	void LeveledListEntryCountWarning::CheckLeveledLists()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();

		uint16_t listsChecked = 0;
		bool foundBadLL = false;

		for (RE::TESLevItem* levItem : formArray) {
			listsChecked++;

			RE::TESLeveledList* leveledList = levItem->As<RE::TESLeveledList>();
			if (!leveledList) {
				continue;
			}
			int32_t numEntries = Utility::GetNumEntries(leveledList);
			if (numEntries <= 255) {
				continue;
			}

			logger::info("LeveledListEntryCountWarning-> LeveledList (FormID: {:08X}, EditorID: {}) has {} entries."sv,
				levItem->GetFormID(), levItem->GetFormEditorID(), numEntries);
			foundBadLL = true;
		}
		if (foundBadLL) {
			RE::ConsoleLog::GetSingleton()->AddString("MentatsF4SE -> LeveledListEntryCountWarning -> Warning: At least 1 leveled list has over 255 entries. Check the log at Documents/My Games/Fallout4/F4SE/MentatsF4SE.log\n");
		}
		logger::info("LeveledListEntryCountWarning -> ListsChecked: {}, FoundBadLL: {}."sv, listsChecked, foundBadLL);
	}
}

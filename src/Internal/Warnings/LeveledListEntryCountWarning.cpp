#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"

namespace Internal::Warnings
{
	void LeveledListEntryCountWarning::Install() noexcept
	{
		logger::info("LeveledListEntryCountWarning -> Warning installing..."sv);

		if (!Config::bLeveledListEntryCountWarning.GetValue()) {
			logger::info("Warning aborted: LeveledListEntryCountWarning. Reason: Warning was disabled in config."sv);
			return;
		}

		CheckLeveledLists();

		logger::info("LeveledListEntryCountWarning -> Warning installed."sv);
	}

	void LeveledListEntryCountWarning::CheckLeveledLists() noexcept
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			return;
		}
		auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();
		if (!formArray) {
			return;
		}

		uint16_t listsChecked = 0;
		bool foundBadLL = false;

		for (RE::TESLevItem* levItem : formArray) {
			if (!levItem) {
				continue;
			}
			RE::TESLeveledList* leveledList = levItem->As<RE::TESLeveledList>();
			if (!leveledList) {
				continue;
			}

			listsChecked++;

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
		logger::info("LeveledListEntryCountWarning -> ListsChecked: {}, FoundBadLeveledList: {}."sv, listsChecked, foundBadLL);
	}
}

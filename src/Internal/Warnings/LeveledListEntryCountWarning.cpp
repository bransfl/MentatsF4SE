#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility/Utility.hpp"

namespace Internal::Warnings
{
	void LeveledListEntryCountWarning::CheckLeveledLists()
	{
		logger::info("LeveledListEntryCountWarning -> Warning installing..."sv);

		if (!Config::bLeveledListEntryCountWarning.GetValue()) {
			logger::info("Warning aborted: LeveledListEntryCountWarning. Reason: Process was disabled in toml file."sv);
			return;
		}

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();
		uint16_t listsChecked = 0;
		bool foundBadLL = false;

		for (auto* levItem : formArray) {
			listsChecked++;

			auto* leveledList = levItem->As<RE::TESLeveledList>();
			if (!leveledList) {
				continue;
			}
			int8_t numEntries = Utility::GetNumEntries(leveledList);
			if (numEntries <= 255) {
				continue;
			}

			logger::info("LeveledListEntryCountWarning-> LeveledList (FormID: {:08X}, EditorID: {}) has {} entries."sv,
				levItem->GetFormID(), levItem->GetFormEditorID(), numEntries);
			foundBadLL = true;
		}
		if (foundBadLL) {
			RE::ConsoleLog::GetSingleton()->AddString("EngineFixesF4SE -> LeveledListEntryCountWarning -> Warning: At least 1 leveled list has over 255 entries. Check the log at Documents/My Games/Fallout4/F4SE/EngineFixesF4SE.log\n");
		}
		logger::info("LeveledListEntryCountWarning -> ListsChecked: {}, FoundBadLL: {}."sv, listsChecked, foundBadLL);

		logger::info("LeveledListEntryCountWarning -> Warning installed."sv);
	}
}

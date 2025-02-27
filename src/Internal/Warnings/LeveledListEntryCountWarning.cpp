#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Warnings::LeveledListEntryCountWarning
{
	void CheckLeveledLists()
	{
		logger::info("LeveledListEntryCountWarning -> Checking LeveledLists"sv);

		if (!Config::bLeveledListEntryCountWarning.GetValue()) {
			logger::info("Warning aborted: LeveledListEntryCountWarning. Reason: Process was disabled in ini file."sv);
			return;
		}

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::TESLevItem>();
		uint16_t listsChecked = 0;
		bool foundBadLL = false;

		for (auto* form : formArray) {
			listsChecked++;
			auto* leveledList = form->As<RE::TESLeveledList>();
			if (!leveledList) {
				continue;
			}
			int8_t numEntries = GetNumEntries(leveledList);
			if (!(numEntries == 0 || numEntries == 255)) {
				continue;
			}
			if (numEntries <= 255) {
				continue;
			}

			logger::info("LeveledListEntryCountWarning-> LeveledList {} has {} entries", form->GetFormID(), numEntries);
			foundBadLL = true;
		}
		if (foundBadLL) {
			logger::warn("LeveledListEntryCountWarning -> Warning: At least 1 leveled list has over 255 entries in the plugin record. Check the log at Documents/My Games/Fallout4/F4SE/EngineFixesF4SE.log"sv);
		}
		logger::info("LeveledListEntryCountWarning -> ListsChecked: {}", listsChecked);
	}

	// returns the total amount of leveledlist entries
	int8_t GetNumEntries(RE::TESLeveledList* leveledList)
	{
		int8_t ret = leveledList->baseListCount + leveledList->scriptListCount;
		logger::debug(FMT_STRING("LeveledListEntryCountWarning -> GetNumEntries for LeveledList NAME_HERE resulted in {}"), ret);
		return ret;
	}
}

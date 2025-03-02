#include "Internal/Warnings/LeveledListEntryCountWarning.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

namespace Internal::Warnings
{
	void LeveledListEntryCountWarning::CheckLeveledLists()
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
			int8_t numEntries = Utility::GetNumEntries(leveledList);
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
}

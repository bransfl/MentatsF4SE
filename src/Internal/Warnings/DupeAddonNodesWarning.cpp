#include "Internal/Warnings/DupeAddonNodesWarning.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Utility/Utility.hpp"

namespace Internal::Warnings
{
	std::unordered_map<std::uint32_t, RE::BGSAddonNode*> nodeMap;

	void DupeAddonNodesWarning::Install() noexcept
	{
		logger::info(FMT_STRING("Warning installing: DupeAddonNodesWarning."sv));

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in config file."sv));
			return;
		}

		ClearNodeMap();

		CheckDupeAddonNodes();

		logger::info(FMT_STRING("Warning installed: DupeAddonNodesWarning."sv));
	}

	void DupeAddonNodesWarning::CheckDupeAddonNodes()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::BGSAddonNode>();
		uint32_t nodesChecked = 0;
		uint32_t nodeErrors = 0;

		for (auto* addonNode : formArray) {
			if (!addonNode) {
				continue;
			}

			addonNode = addonNode->As<RE::BGSAddonNode>();
			if (!addonNode) {
				continue;
			}

			nodesChecked++;
			const auto res = nodeMap.insert(std::make_pair(addonNode->index, addonNode));

			if (!res.second) {
				const auto currentNode = (*res.first).second;
				if (currentNode != addonNode && currentNode->formID != addonNode->formID) {
					nodeErrors++;
					logger::warn(FMT_STRING("DupeAddonNodesWarning -> Duplicate AddonNode Index found at (FormID: {:08X} in Plugin: {}) AND (FormID {:08X} in plugin {}). The AddonNode index was {}."),
						currentNode->formID, Utility::GetModName(currentNode, false), addonNode->formID, Utility::GetModName(addonNode, false), addonNode->index);
					RE::ConsoleLog::GetSingleton()->AddString("EngineFixesF4SE -> DupeAddonNodesWarning -> An AddonNode duplicate index was found, check EngineFixesF4SE.log for more information.\n");
				}
			}
		}

		logger::info(FMT_STRING("DupeAddonNodesWarning -> CheckDupeAddonNodes finished, NodesChecked: {}, NodeErrors: {}"), nodesChecked, nodeErrors);
	}

	void DupeAddonNodesWarning::ClearNodeMap()
	{
		logger::info(FMT_STRING("DupeAddonNodesWarning -> Clearing node map..."sv));

		if (!nodeMap.empty()) {
			nodeMap.clear();
			logger::info(FMT_STRING("DupeAddonNodesWarning -> Node map cleared."sv));
		}
		else {
			logger::info(FMT_STRING("DupeAddonNodesWarning -> Node was already cleared."sv));
		}
	}
}

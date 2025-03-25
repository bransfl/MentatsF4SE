#include "Internal/Warnings/DupeAddonNodesWarning.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility.hpp"

namespace Internal::Warnings
{
	std::unordered_map<uint32_t, RE::BGSAddonNode*> nodeMap;

	void DupeAddonNodesWarning::Install() noexcept
	{
		logger::info("Warning installing: DupeAddonNodesWarning."sv);

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("Fix aborted: DupeAddonNodesWarning. Reason: Fix was disabled in config file."sv);
			return;
		}

		CheckDupeAddonNodes();

		ClearNodeMap();

		logger::info("Warning installed: DupeAddonNodesWarning."sv);
	}

	void DupeAddonNodesWarning::CheckDupeAddonNodes()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto& formArray = dataHandler->GetFormArray<RE::BGSAddonNode>();
		uint32_t nodesChecked = 0;
		uint32_t nodeErrors = 0;

		ClearNodeMap();

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
					logger::warn("DupeAddonNodesWarning -> Duplicate AddonNode Index found at (FormID: {:08X} in Plugin: {}) AND (FormID {:08X} in plugin {}). The AddonNode index was {}."sv,
						currentNode->formID, Utility::GetModName(currentNode, false),
						addonNode->formID, Utility::GetModName(addonNode, false), addonNode->index);
					RE::ConsoleLog::GetSingleton()->AddString("MentatsF4SE -> DupeAddonNodesWarning -> An duplicate AddonNode index was found, check MentatsF4SE.log for more information.\n");
				}
			}
		}

		logger::info("DupeAddonNodesWarning -> CheckDupeAddonNodes finished. NodesChecked: {}, NodeErrors: {}."sv, nodesChecked, nodeErrors);
	}

	void DupeAddonNodesWarning::ClearNodeMap()
	{
		if (!nodeMap.empty()) {
			nodeMap.clear();
		}
	}
}

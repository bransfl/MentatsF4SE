#include "Internal/Warnings/DupeAddonNodesWarning.hpp"

namespace Internal::Warnings
{
	std::unordered_map<uint32_t, RE::BGSAddonNode*> nodeMap;

	void DupeAddonNodesWarning::Install() noexcept
	{
		logger::info("Warning installing: DupeAddonNodesWarning."sv);

		if (!Config::bDupeAddonNodesWarning.GetValue()) {
			logger::info("Warning aborted: DupeAddonNodesWarning. Reason: Fix was disabled in config."sv);
			return;
		}

		CheckDupeAddonNodes();

		ClearNodeMap();

		logger::info("Warning installed: DupeAddonNodesWarning."sv);
	}

	void DupeAddonNodesWarning::CheckDupeAddonNodes() noexcept
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
					logger::warn("\tDupeAddonNodesWarning -> Duplicate AddonNode index found: {}. Index was found at (Form: {} in Plugin: {}) AND (Form: {} in Plugin {})."sv,
						addonNode->index,
						Utility::GetFormInfo(currentNode), Utility::GetModName(currentNode, false),
						Utility::GetFormInfo(addonNode), Utility::GetModName(addonNode, false));
					RE::ConsoleLog::GetSingleton()->AddString("MentatsF4SE -> DupeAddonNodesWarning -> An duplicate AddonNode index was found, do not ignore this warning. Check MentatsF4SE.log for more information.\n");
				}
			}
		}

		logger::info("\tDupeAddonNodesWarning -> CheckDupeAddonNodes finished. NodesChecked: {}, NodeErrors: {}."sv, nodesChecked, nodeErrors);
	}

	void DupeAddonNodesWarning::ClearNodeMap() noexcept
	{
		if (!nodeMap.empty()) {
			nodeMap.clear();
		}
	}
}

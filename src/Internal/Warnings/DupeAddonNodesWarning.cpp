#include "Internal/Warnings/DupeAddonNodesWarning.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Warnings::DupeAddonNodesWarning
{
	std::unordered_map<std::uint32_t, RE::BGSAddonNode*> nodeMap;

	void Install() noexcept
	{
		logger::info("Warning installing: DupeAddonNodesWarning."sv);

		if (!Config::bLeveledListCrashFix.GetValue()) {
			logger::info("Fix aborted: LeveledListCrashFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		CheckDupeAddonNodes();

		logger::info("Warning installed: DupeAddonNodesWarning."sv);
	}

	void CheckDupeAddonNodes()
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
			nodesChecked++;
			const auto res = nodeMap.insert(std::make_pair(addonNode->index, addonNode));

			if (!res.second) {
				const auto current = (*res.first).second;
				if (current != addonNode && current->formID != addonNode->formID) {
					nodeErrors++;
					std::string_view modNameCurrent = GetModName(current, false);
					std::string_view modNameAddonNode = GetModName(addonNode, false);
					logger::warn(FMT_STRING("DupeAddonNodesWarning -> Duplicate AddonNode Index found at FormID {:08X} in plugin {} AND FormID {:08X} in plugin {}, the index was {}."),
						current->formID, modNameCurrent, addonNode->formID, modNameAddonNode, addonNode->index);
					RE::ConsoleLog::GetSingleton()->AddString("EngineFixesF4SE -> DupeAddonNodesWarning -> An AddonNode duplicate index was found, check the log for more information.\n");
				}
			}
		}

		logger::info(FMT_STRING("DupeAddonNodesWarning -> CheckDupeAddonNodes finished, NodesChecked: {}, NodeErrors: {}"), nodesChecked, nodeErrors);
	}

	void ClearNodeMap()
	{
		logger::info("DupeAddonNodesWarning -> Clearing node map..."sv);

		nodeMap.clear();

		logger::info("DupeAddonNodesWarning -> Node map cleared."sv);
	}

	std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified)
	{
		const auto index = a_lastModified ? -1 : 0;
		const auto* file = a_form->GetFile(index);
		return file ? file->filename : "MODNAME_NOT_FOUND"sv;
	}
}

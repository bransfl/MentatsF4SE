#pragma once

namespace Internal::Warnings::DupeAddonNodesWarning
{
	void Install() noexcept;

	void CheckDupeAddonNodes();

	void ClearNodeMap();

	std::string_view GetModName(RE::TESForm* a_form, bool a_lastModified);
}

#pragma once

namespace Internal::Warnings
{
	class DupeAddonNodesWarning
	{
	public:
		static void Install() noexcept;

		static void CheckDupeAddonNodes();

		static void ClearNodeMap();
	};
}

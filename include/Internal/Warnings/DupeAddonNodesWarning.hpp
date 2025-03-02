#pragma once

namespace Internal::Warnings
{
	class DupeAddonNodesWarning
	{
	public:
		// installs the warning
		static void Install() noexcept;

		// checks and reports any duplicate addon node indexes
		static void CheckDupeAddonNodes();

		// resets the nodemap to free up memory
		static void ClearNodeMap();
	};
}

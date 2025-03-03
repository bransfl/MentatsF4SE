#pragma once

namespace Internal::Warnings
{
	class RogueMasterWarning
	{
	public:
		static void Install() noexcept;
	private:
		static void CheckRogueMasters();
	};
}

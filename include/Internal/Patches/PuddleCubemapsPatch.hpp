#pragma once

namespace Internal::Patches
{
	class PuddleCubemapsPatch
	{
	public:
		static void Install() noexcept;

	private:
		static void Fix();
	};
}

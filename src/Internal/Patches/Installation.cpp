#include "Internal/Patches/Installation.hpp"

namespace Internal::Patches
{
	void Installation::Install() noexcept
	{
		logger::info("Patches installing..."sv);

		// PuddleCubemapsPatch is handled in Messaging

		logger::info("Patches installed."sv);
	}
}

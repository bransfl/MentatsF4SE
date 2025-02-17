#include "Internal/Patches/Installation.hpp"
#include "Internal/Patches/LoadEditorIDs.hpp"

namespace Internal::Patches
{
	void Install() noexcept
	{
		logger::debug("Installing patches"sv);

		LoadEditorIDs::Install();
	}
}

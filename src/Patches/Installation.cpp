#include "Internal/Patches/Installation.hpp"
#include "Internal/Patches/LoadEditorIDs.hpp"

namespace Internal::Patches
{
	void Install() noexcept
	{
		logger::info("Installing patches"sv);

		LoadEditorIDs::Install();
	}
}

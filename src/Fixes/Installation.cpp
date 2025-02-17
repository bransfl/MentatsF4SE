#include "Internal/Fixes/Installation.hpp"
#include "Internal/Fixes/ApplySpells.hpp"

namespace Internal::Fixes
{
	void Install() noexcept
	{
		logger::info("Installing fixes"sv);

		ApplySpells::Install();
	}
}

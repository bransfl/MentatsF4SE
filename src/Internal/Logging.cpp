#include "Internal/Logging.hpp"

namespace Internal
{
	void Logging::LogUserVersionInfo()
	{
		LogSeparator();

		logger::info("VERSION CHECK:"sv);

		logger::info("\tUser game version: {}."sv, (REL::Module::IsNG() ? "Next-Gen"sv : "Old-Gen"sv));

		logger::info("\tUser F4SE version: {}."sv, F4SE::GetF4SEVersion());

		// game/f4se version match check
		bool versionMatch = false;
		if (REL::Module::IsNG()) {
			versionMatch = F4SE::GetF4SEVersion().string() == "0.7.2.0"sv;
		}
		else {
			versionMatch = F4SE::GetF4SEVersion().string() == "0.6.23.0"sv;
		}
		logger::info("\tUser has correct Game and F4SE version: {}."sv, versionMatch);

		// address library version check
		bool hasAddressLibrary = false;
		if (REL::Module::IsNG()) {
			std::filesystem::exists("Data/F4SE/Plugins/version-1-10-984-0.bin"sv) ? hasAddressLibrary = true : hasAddressLibrary = false;
		}
		else {
			std::filesystem::exists("Data/F4SE/Plugins/version-1-10-163-0.bin"sv) ? hasAddressLibrary = true : hasAddressLibrary = false;
		}
		logger::info("\tUser has the correct version of Address Library: {}."sv, hasAddressLibrary);
	}

	void Logging::LogSeparator() noexcept
	{
		logger::info("---------------------------------------------"sv);
	}

	std::string_view Logging::ReasonToString(Logging::REASON a_reason) noexcept
	{
		switch (a_reason) {
			case REASON::kDisabledTemporarily: {
				return "Temporarily disabled"sv;
				break;
			}
			case REASON::kDisabledInConfig: {
				return "Disabled in config file"sv;
				break;
			}
			case REASON::kDisabledInNG: {
				return "Disabled for Next-Gen"sv;
				break;
			}
			case REASON::kDisabledInOG: {
				return "Disabled for Last-Gen"sv;
				break;
			}
			case REASON::kIncompatiblePluginLoaded: {
				return "Incompatible plugin was loaded"sv;
				break;
			}
			default: {
				return "UNKNOWN"sv;
				break;
			}
		};
	}
}

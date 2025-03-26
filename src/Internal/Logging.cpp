#include "Internal/Logging.hpp"

namespace Internal
{
	static inline constexpr std::string_view addressLibraryPathNG = "Data/F4SE/Plugins/version-1-10-984-0.bin";
	static inline constexpr std::string_view addressLibraryPathOG = "Data/F4SE/Plugins/version-1-10-163-0.bin";

	void Logging::LogUserVersionInfo()
	{
		LogSeparator();

		logger::info("VERSION CHECK:"sv);

		logger::info("\tUser F4SE version: {}."sv, F4SE::GetF4SEVersion());

		logger::info("\tUser game version: {}."sv, (REL::Module::IsNG() ? "Next-Gen"sv : "Old-Gen"sv));

		// game:f4se version check
		bool versionMatch = false;
		if (REL::Module::IsNG()) {
			if (F4SE::GetF4SEVersion().string() == "0.7.2.0"sv) {
				versionMatch = true;
			}
		}
		else {
			if (F4SE::GetF4SEVersion().string() == "0.6.23.0"sv) {
				versionMatch = true;
			}
		}
		logger::info("\tUser has correct Game and F4SE version: {}."sv, versionMatch);

		// address library version check
		bool hasAddressLibrary = false;
		if (REL::Module::IsNG()) {
			std::filesystem::exists(addressLibraryPathNG) ? hasAddressLibrary = true : hasAddressLibrary = false;
		}
		else {
			std::filesystem::exists(addressLibraryPathOG) ? hasAddressLibrary = true : hasAddressLibrary = false;
		}
		logger::info("\tUser has the correct version of Address Library: {}."sv, hasAddressLibrary);
	}

	void Logging::LogSeparator()
	{
		logger::info("---------------------------------------------"sv);
	}
}

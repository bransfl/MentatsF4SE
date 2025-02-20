#include "Internal/Config/Config.hpp"

namespace Internal::Config
{
	static constexpr auto PLUGINS_PATH = "Data/F4SE/Plugins"sv;

	// ini
	static const auto FILE_PATH_INI = fmt::format("{}/{}.ini", PLUGINS_PATH, Plugin::NAME);
	static const auto CUSTOM_FILE_PATH_INI = fmt::format("{}/{}_Custom.ini", PLUGINS_PATH, Plugin::NAME);

	void Load() noexcept
	{
		logger::info("Config loading...");

		auto* ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(FILE_PATH_INI, CUSTOM_FILE_PATH_INI);
		ini->Load();

		logger::info("Config loaded");
	}
}

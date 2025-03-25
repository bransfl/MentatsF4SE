#include "Internal/Config.hpp"

namespace Internal::Config
{
	static constexpr std::string_view PLUGINS_PATH = "Data/F4SE/Plugins"sv;

	static const std::string FILE_PATH_TOML = fmt::format("{}/{}.toml", PLUGINS_PATH, Plugin::NAME);
	static const std::string CUSTOM_FILE_PATH_TOML = fmt::format("{}/{}_Custom.toml", PLUGINS_PATH, Plugin::NAME);

	void Load() noexcept
	{
		REX::TOML::SettingStore* toml = REX::TOML::SettingStore::GetSingleton();
		toml->Init(FILE_PATH_TOML, CUSTOM_FILE_PATH_TOML);
		toml->Load();
	}
}

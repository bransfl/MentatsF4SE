#include "Internal/Config/Config.hpp"

namespace Internal::Config
{
	static constexpr auto PLUGINS_PATH = "Data/F4SE/Plugins"sv;

	static const auto FILE_PATH_TOML = fmt::format("{}/{}.toml", PLUGINS_PATH, Plugin::NAME);
	static const auto CUSTOM_FILE_PATH_TOML = fmt::format("{}/{}_Custom.toml", PLUGINS_PATH, Plugin::NAME);

	void Load() noexcept
	{
		auto* toml = REX::TOML::SettingStore::GetSingleton();
		toml->Init(FILE_PATH_TOML, CUSTOM_FILE_PATH_TOML);
		toml->Load();
	}
}

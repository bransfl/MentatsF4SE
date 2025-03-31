#pragma once

namespace Internal
{
	class Logging
	{
	public:
		/**
		 * @brief Enum class for consistent log formatting between modules.
		 */
		static enum class REASON : uint8_t {
			kDisabledTemporarily = 0,
			kDisabledInConfig = 1,
			kDisabledInNG = 2,
			kDisabledInOG = 3,
			kIncompatiblePluginLoaded = 4,

			kTotal = 5
		};

		/**
		 * @brief Logs the user's game version, f4se version, and address library version.
		 */
		static void LogUserVersionInfo();

		/**
		 * @brief Logs a single line of hyphens.
		 */
		static void LogSeparator() noexcept;

		/**
		 * @brief toString() method for enum REASON.
		 * @param a_reason The reason to convert to a string.
		 * @return The string representation of a_reason.
		 */
		static std::string_view ReasonToString(Logging::REASON a_reason) noexcept;
	};
}

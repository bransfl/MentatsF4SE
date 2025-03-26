#pragma once

namespace Internal
{
	class Logging
	{
	public:
		// static enum class REASON {
		// 	kUnknown = 0,
		// 	kDisabledInConfig = 1,
		// 	kDisabledInNG = 2,
		// 	kDisabledInOG = 3,
		// 	kTotal = 4
		// };

		/**
		 * @brief Logs the user's game version, f4se version, and address library version.
		 */
		static void LogUserVersionInfo();

		/**
		 * @brief Logs a single line of hyphens.
		 */
		static void LogSeparator();
	};
}

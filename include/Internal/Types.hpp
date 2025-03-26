#pragma once

#include <string_view>

namespace Internal
{
	/**
	 * @brief Superclass for all Fixes, Patches, and Warnings.
	 */
	class Module
	{
	public:
		virtual std::string_view GetModuleName() = 0;
		virtual std::string_view GetType() = 0;
	};

	/**
	 * @brief Superclass for all Fixes.
	 */
	class Fix
		: public Module
	{
		static std::string_view Module::GetType()
		{
			return "Fix";
		}
	};

	/**
	 * @brief Superclass for all Patches.
	 */
	class Patch
		: public Module
	{
		static std::string_view Module::GetType()
		{
			return "Patch";
		}
	};

	/**
	 * @brief Superclass for all Warnings.
	 */
	class Warning
		: public Module
	{
		static std::string_view Module::GetType()
		{
			return "Warning";
		}
	};

	/**
	 * @brief Superclass for all module installers.
	 */
	class Installer
	{
	};
}

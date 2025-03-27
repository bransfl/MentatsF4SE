#pragma once

#include <string_view>

namespace Internal
{
	/**
	 * @brief Superclass for all Fixes, Patches, Warnings, and Installers.
	 */
	class Module
	{
	public:
		virtual inline std::string_view GetModuleName() = 0;
		virtual inline std::string_view GetType() = 0;
	};

	/**
	 * @brief Superclass for all Fixes.
	 */
	class Fix
		: public Module
	{
	public:
		static inline constexpr std::string_view Module::GetType() noexcept
		{
			return std::string_view("Fix");
		}
	};

	/**
	 * @brief Superclass for all Patches.
	 */
	class Patch
		: public Module
	{
	public:
		static inline constexpr std::string_view Module::GetType() noexcept
		{
			return std::string_view("Patch");
		}
	};

	/**
	 * @brief Superclass for all Warnings.
	 */
	class Warning
		: public Module
	{
	public:
		static inline constexpr std::string_view Module::GetType() noexcept
		{
			return std::string_view("Warning");
		}
	};

	/**
	 * @brief Superclass for all module installers.
	 */
	class Installer
		: public Module
	{
	public:
		static inline constexpr std::string_view Module::GetType() noexcept
		{
			return std::string_view("Installer");
		}
	};
}

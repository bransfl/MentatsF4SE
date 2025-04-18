#pragma once

namespace Internal
{
	class Papyrus
	{
		/* Papyrus script name */
		static inline constexpr std::string_view SCRIPT_NAME = "MentatsF4SE"sv;

		/**
		 * @brief Checks if Mentats is installed.
		 * @return If MentatsF4SE is installed or not.
		 */
		static bool IsMentatsInstalled(RE::BSScript::IVirtualMachine& a_vm, RE::VMStackID a_vmStackID, std::monostate) noexcept;

		/**
		 * @brief Registers new Papyrus functions with the VM.
		 * @return True if functions were succesfully registered.
		 *		   False if functions were not successfully registered.
		 */
		static bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) noexcept;

		/**
		 * @brief Callback function for Papyrus registration.
		 * @return True if functions were succesfully registered.
		 *		   False if functions were not successfully registered.
		 */
		bool Callback(RE::BSScript::IVirtualMachine* a_vm) noexcept;
	};
}

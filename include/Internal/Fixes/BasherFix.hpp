#pragma once

// fixes entrypoint ModOutgoingLimbBashDamage so it actually works
// note - this will likely require RE, and wont be made until RE is done.
namespace Internal::Fixes
{
	class BasherFix
		: public Fix
	{
	public:
		/**
		 * @brief Installs the fix.
		 */
		void Install() noexcept;

		/**
		 * @brief Class name method.
		 */
		static std::string_view Module::GetModuleName()
		{
			return "BasherFix"sv;
		}
	};

	// RE::BGSEntryPoint::ENTRY_POINT::kModBashingDamage = 0x1A,
}

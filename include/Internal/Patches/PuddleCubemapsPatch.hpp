#pragma once

namespace Internal::Patches
{
	class PuddleCubemapsPatch
		: public Patch
	{
	public:
		/**
		 * @brief Installs the patch.
		 */
		static void Install() noexcept;

		/**
		 * @brief Class name method.
		 */
		static std::string_view Module::GetModuleName()
		{
			return "PuddleCubemapsPatch"sv;
		}

	private:
		/**
		 * @brief Patches all loaded RE::TESWorldspace forms to set their waterEnvMap member to none.
		 */
		static void Patch();
	};
}

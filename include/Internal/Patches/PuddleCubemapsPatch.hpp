#pragma once

namespace Internal::Patches
{
	class PuddleCubemapsPatch
	{
	public:
		/**
		 * @brief Installs the patch.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Patches all loaded RE::TESWorldspace forms to set their waterEnvMap member to none.
		 */
		static void Patch();
	};
}

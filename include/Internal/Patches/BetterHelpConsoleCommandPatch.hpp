#pragma once

namespace Internal
{
	// CREDIT - Ryan-rsm-McKenzie - Console Commands Extender - (MIT) https://github.com/clayne/CCExtenderF4
	class BetterHelpConsoleCommandPatch
	{
	public:
		enum class Filter
		{
			kAll,
			kFunctions,
			kSettings,
			kGlobals,
			kForms,

			kTotal
		};

		/**
		 * @brief todo
		 */
		static void Install() noexcept;

		/**
		 * @brief todo
		 */
		[[nodiscard]] static inline const std::string& HelpString();

		/**
		 * @brief todo
		 */
		[[nodiscard]] static inline auto Parse(
			const RE::SCRIPT_PARAMETER* a_params,
			const char* a_compiledParams,
			uint32_t& a_offset,
			RE::TESObjectREFR* a_ref,
			RE::TESObjectREFR* a_container,
			RE::Script* a_script,
			RE::ScriptLocals* a_scriptLocals)
			-> std::tuple<std::optional<std::string>,
				std::optional<BetterHelpConsoleCommandPatch::Filter>,
				std::optional<std::string>>;

		/**
		 * @brief todo
		 */
		static inline void Print(stl::zstring a_zstring);

		/**
		 * @brief todo
		 */
		// template <class T, size_t N, class UnaryFunctor>
		// [[nodiscard]] inline auto Enumerate(
		// 	std::string_view a_matchstring,
		// 	std::span<T, N> a_src,
		// 	UnaryFunctor a_callback);
	};
}

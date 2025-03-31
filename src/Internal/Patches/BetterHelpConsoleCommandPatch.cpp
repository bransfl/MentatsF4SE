#include "Internal/Patches/BetterHelpConsoleCommandPatch.hpp"
#include <boost/algorithm/searching/knuth_morris_pratt.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/vector.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace Internal
{
	void BetterHelpConsoleCommandPatch::Install() noexcept
	{
		logger::info("Patch installing: BetterHelpConsoleCommandPatch."sv);

		if (!Config::bBetterHelpConsoleCommandPatch.GetValue()) {
			logger::info("Fix aborted: BetterHelpConsoleCommandPatch. Reason: Fix was disabled in config."sv);
			return;
		}

		if (REX::W32::GetModuleHandleW(L"CCExtenderF4.dll")) {
			logger::info("Patch aborted: BetterHelpConsoleCommandPatch. Reason: Mod was installed: CCExtenderF4.dll."sv);
			return;
		}

		logger::info("Patch installed: BetterHelpConsoleCommandPatch."sv);
	}

	[[nodiscard]] inline const std::string& BetterHelpConsoleCommandPatch::HelpString()
	{
		static auto help = []() {
			std::string buf;
			buf += "\"Help\" <expr>"sv;
			buf += "\n\t<expr> ::= <empty> | \" \" <matchstring> | \" \" <matchstring> \" \" <filter> | \" \" <matchstring> \" \" <filter> \" \" <form-type>"sv;
			buf += "\n\t<matchstring> ::= <string> ; The string to filter results with"sv;
			buf += "\n\t<filter> ::= <integer>"sv;
			buf += "\n\t\t; 0 - All"sv;
			buf += "\n\t\t; 1 - Functions"sv;
			buf += "\n\t\t; 2 - Settings"sv;
			buf += "\n\t\t; 3 - Globals"sv;
			buf += "\n\t\t; 4 - Forms"sv;
			buf += "\n\t<form-type> ::= <string> ; The form type to filter form results with"sv;
			return buf;
		}();
		return help;
	}

	[[nodiscard]] inline auto BetterHelpConsoleCommandPatch::Parse(
		const RE::SCRIPT_PARAMETER* a_params,
		const char* a_compiledParams,
		uint32_t& a_offset,
		RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_container,
		RE::Script* a_script,
		RE::ScriptLocals* a_scriptLocals)
		-> std::tuple<
			std::optional<std::string>,
			std::optional<BetterHelpConsoleCommandPatch::Filter>,
			std::optional<std::string>>
	{
		std::array<char, 0x200> matchstring{ '\0' };
		int32_t filter = -1;
		std::array<char, 0x200> formtype{ '\0' };

		RE::Script::ParseParameters(
			a_params,
			a_compiledParams,
			a_offset,
			a_ref,
			a_container,
			a_script,
			a_scriptLocals,
			matchstring.data(),
			std::addressof(filter),
			formtype.data());

		std::tuple<std::optional<std::string>,
			std::optional<BetterHelpConsoleCommandPatch::Filter>,
			std::optional<std::string>>
			results;

		if (matchstring[0] != '\0') {
			std::get<0>(results) = matchstring.data();
		}

		if (filter != -1) {
			std::get<1>(results) = static_cast<BetterHelpConsoleCommandPatch::Filter>(filter);
		}

		if (formtype[0] != '\0') {
			std::get<2>(results) = formtype.data();
		}

		return results;
	}

	inline void BetterHelpConsoleCommandPatch::Print(stl::zstring a_zstring)
	{
		const auto consoleLog = RE::ConsoleLog::GetSingleton();
		if (consoleLog) {
			consoleLog->AddString(a_zstring.data());
		}
	}

	// template <class T, size_t N, class UnaryFunctor>
	// [[nodiscard]] inline auto Enumerate(
	// 	std::string_view a_matchstring,
	// 	std::span<T, N> a_src,
	// 	UnaryFunctor a_callback)
	// {
	// 	boost::container::vector<bool> result(a_src.size(), false);
	// 	const boost::algorith::knuth_morris_prat kmp{ a_matchstring.begin(), a_matchstring.end() };

	// 	std::for_each_n(
	// 		std::execution::parallel_unsequenced_policy{},
	// 		a_src.begin(),
	// 		a_src.size(),
	// 		[&](auto&& a_elem) noexcept {
	// 		for (const auto& haystack : a_callback(a_elem)) {
	// 			const auto [first, last] = kmp(
	// 				stl::cistring_iterator { haystack.begin() },
	// 				stl::cistring_iterator{ haystack.end() });
	// 			if (first != last) {
	// 				const auto pos = std::addressof(a_elem) - a_src.data();
	// 				results[pos] = true;
	// 			}
	// 		}
	// 	});

	// 	std::vector<
	// 		std::conditional_t<
	// 			std::is_pointer_v<T>,
	// 			std::remove_const_t<T>,
	// 			T*>>
	// 		matched;
	// 	matched.reserve(a_src.size());
	// 	for (size_t i = 0; i < results.size(); ++i) {
	// 		if (results[i]) {
	// 			if (constexpr(std::is_pointer_v<T>)) {
	// 				matched.push_back(a_src[i]);
	// 			}
	// 			else {
	// 				matched.push_back(a_src.data() + i);
	// 			}
	// 		}
	// 	}
	// 	return matched;
	// }
}

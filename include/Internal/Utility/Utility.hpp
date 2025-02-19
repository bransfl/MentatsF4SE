#pragma once

namespace Internal::Utility
{
	namespace Console
	{
		static constexpr auto LINE_SEPARATOR = '\n';
		static constexpr auto COMMAND_SEPARATOR = ';';
		static constexpr auto COMMAND_HISTORY_PATH = "root1.AnimHolder_mc.Menu_mc.CommandHistory.text"sv;

		bool ExecuteCommand(const std::string& a_command);
	}
}
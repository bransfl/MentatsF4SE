#pragma once

namespace Internal::Utility
{
	namespace Console
	{
		void ExecuteCommand(std::string_view a_command, RE::TESObjectREFR* a_targetRef, bool a_silent);
	}
}

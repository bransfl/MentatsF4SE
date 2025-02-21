#pragma once

namespace Internal::Fixes::OutfitRedressFix
{
	// installs the fix
	void Install() noexcept;

	// the hook
	std::int64_t Hook_Set3DUpdateFlag(RE::AIProcess* a_process, RE::RESET_3D_FLAGS a_flags);

	// the hooker
	void RegisterHook(F4SE::Trampoline& trampoline);
}

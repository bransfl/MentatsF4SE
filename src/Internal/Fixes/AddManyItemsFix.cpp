#include "Internal/Fixes/AddManyItemsFix.hpp"
#include "f4se_common/Relocation.h"

namespace Internal::Fixes
{

	void AddManyItemsFix::Install() noexcept
	{
		logger::info("Fix installing: AddManyItemsFix."sv);

		if (!Config::bAddManyItemsFix.GetValue()) {
			logger::info("Fix aborted: AddManyItemsFix. Reason: Fix was disabled in config."sv);
			return;
		}

		if (REX::W32::GetModuleHandleW(L"AddFFFFFix.dll")) {
			logger::warn("Fix aborted: AddManyItemsFix. Reason: Mod was installed: AddFFFFFix.dll."sv);
			return;
		}

		Fix();

		logger::info("Fix installed: AddManyItemsFix."sv);
	}

	void AddManyItemsFix::Fix()
	{
		if (REL::Module::IsNG()) {
			// NG patch
			uint8_t buf[] = { 0x44, 0x8B, 0x44, 0x24, 0x70, 0x90 };
			REL::safe_write(RelocationManager::s_baseAddr + 0x004AEF51, buf, sizeof(buf));
			// SafeWriteBuf(RelocationManager::s_baseAddr + 0x003FBF3E, buf, sizeof(buf));	NG
		}
		else {
			// OG patch
			uint8_t buf[] = { 0x44, 0x8B, 0x44, 0x24, 0x74, 0x90 };
			REL::safe_write(RelocationManager::s_baseAddr + 0x003FBF3E, buf, sizeof(buf));
			// SafeWriteBuf(RelocationManager::s_baseAddr + 0x004AEF51, buf, sizeof(buf));	OG
		}
	}
}

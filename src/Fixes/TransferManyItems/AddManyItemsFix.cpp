#include "Internal/Fixes/TransferManyItems/AddManyItemsFix.hpp"
#include "Internal/Config/Config.hpp"
#include "f4se_common/Relocation.h"

namespace Internal::Fixes::TransferManyItems::AddManyItemsFix
{

	void Install() noexcept
	{
		logger::info("Fix installing: AddManyItemsFix."sv);

		if (!Config::bAddManyItemsFix.GetValue()) {
			logger::info("Fix aborted: AddManyItemsFix. Reason: Game version was NG."sv);
			return;
		}
		if (REX::W32::GetModuleHandleW(L"AddFFFFFix.dll")) { // not a typo, their dll has 5 F's
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Fix aborted: AddManyItemsFix. Reason: Mod 'Add FFFF Fix' was detected. This fix is not necessary with this mod installed.\n");
			logger::warn("Fix aborted: AddManyItemsFix. Reason: Mod was installed: AddFFFFFix.dll."sv);
			return;
		}

		if (REL::Module::IsNG()) {
			// NG patch - buf[4] is different
			uint8_t buf[] = { 0x44, 0x8B, 0x44, 0x24, 0x70, 0x90 };
			REL::safe_write(RelocationManager::s_baseAddr + 0x004AEF51, buf, sizeof(buf));
		}
		else {
			// OG patch
			uint8_t buf[] = { 0x44, 0x8B, 0x44, 0x24, 0x74, 0x90 };
			REL::safe_write(RelocationManager::s_baseAddr + 0x003FBF3E, buf, sizeof(buf));
		}

		logger::info("Fix installed: AddManyItemsFix."sv);
	}
}

// SafeWriteBuf(RelocationManager::s_baseAddr + 0x004AEF51, buf, sizeof(buf));	OG
// SafeWriteBuf(RelocationManager::s_baseAddr + 0x003FBF3E, buf, sizeof(buf));	NG

#include "Internal/Fixes/TransferManyItems/AddManyItems.hpp"
#include "Internal/Config/Config.hpp"
#include "f4se_common/Relocation.h"
#include "f4se_common/SafeWrite.h"

namespace Internal::Fixes::TransferManyItems::AddManyItems
{

	void Install() noexcept
	{
		logger::info("Fix installing: AddManyItems.");

		if (!Config::bAddManyItems.GetValue()) {
			logger::info("Fix aborted: AddManyItems. Reason: Game version was NG.");
			return;
		}

		uint8_t buf[] = { 0x44, 0x8B, 0x44, 0x24, 0x74, 0x90 };

		// this patch works on both versions of the game
		if (REL::Module::IsNG()) {
			// NG patch
			buf[4] = 0x70;
			SafeWriteBuf(RelocationManager::s_baseAddr + 0x004AEF51, buf, sizeof(buf));
		}
		else {
			// OG patch
			SafeWriteBuf(RelocationManager::s_baseAddr + 0x003FBF3E, buf, sizeof(buf));
		}

		logger::info("Fix installed: AddManyItems.");
	}
}

#include "Internal/Fixes/TransferManyItems/AddManyItems.hpp"
#include "Internal/Config/Config.hpp"
#include "f4se_common/Relocation.h"
#include "f4se_common/SafeWrite.h"

namespace Internal::Fixes::TransferManyItems::AddManyItems
{

	void Install() noexcept
	{
		if (!Config::bAddManyItems.GetValue()) {
			logger::info("AddManyItems -> Fix was disabled in the ini file. Fix aborted.");
			return;
		}

		uint8_t buf[] = { 0x44, 0x8B, 0x44, 0x24, 0x74, 0x90 };

		if (!REL::Module::IsNG()) {
			logger::info("AddManyItems -> Game version is OG.");
			// OG patch
			SafeWriteBuf(RelocationManager::s_baseAddr + 0x003FBF3E, buf, sizeof(buf));
		}
		else {
			logger::info("AddManyItems -> Game version is NG.");
			// NG patch
			buf[4] = 0x70;
			SafeWriteBuf(RelocationManager::s_baseAddr + 0x004AEF51, buf, sizeof(buf));
		}

		logger::info("AddManyItems -> Patch applied.");
	}
}

#include "Internal/Fixes/TransferManyItems/DropManyItems.hpp"

// REL::Relocation<uintptr_t> ptr_ExtraDataList_ctor_OG{ REL::ID(526160) };
// REL::Relocation<uintptr_t> ptr_ExtraDataList_ctor_NG{ REL::ID(2221088) };

// REL::Relocation<uintptr_t> ptr_ExtraDataList_CopyList_OG{ REL::ID(533056) };
// REL::Relocation<uintptr_t> ptr_ExtraDataList_CopyList_NG{ REL::ID(2233536) };

// REL::Relocation<uintptr_t> ptr_ExtraDataList_SetCount_OG{ REL::ID(552880) };
// REL::Relocation<uintptr_t> ptr_ExtraDataList_SetCount_NG{ REL::ID(2256640) };

// REL::Relocation<uintptr_t> ptr_DropItemIntoWorld_Original_OG{ REL::ID(4171136) };
// REL::Relocation<uintptr_t> ptr_DropItemIntoWorld_Original_NG{ REL::ID(4904928) };

#define CastAndAssign(dest) dest = (_##dest)(RelocationManager::s_baseAddr + dest##_Addr[index])

namespace Internal::Fixes::TransferManyItems::DropManyItems
{
	void Install() noexcept
	{
		logger::info("Fix installing: DropManyItems.");

		// REL::Relocation<ExtraDataList_CopyList_OG> jumpLocation{ REL::ID(1394576), 0x1DE };

		logger::info("Fix installed: DropManyItems.");
	}

	// uint32_t* DropItemIntoWorld_Hook(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra)
	// {
	// 	while (count >= 0x8000) {
	// 		count -= 0x7FFF;

	// 		RE::ExtraDataList* list;

	// 		volatile long* listQRefCount = reinterpret_cast<volatile long*>(static_cast<uintptr_t>((list->QRefCount())));
	// 		_InterlockedIncrement(listQRefCount);

	// 		// InterReloc::ExtraDataList_CopyList(extra, list);
	// 		// CastAndAssign(ptr_ExtraDataList_CopyList_OG.address());
	// 		typedef void(ExtraDataList_CopyList_OG)(RE::ExtraDataList, RE::ExtraDataList);
	// 		REL::Relocation<ExtraDataList_CopyList_OG> Function_ExtraDataList_CopyList_OG;

	// 		// InterReloc::ExtraDataList_SetCount(list, 0x7FFF);

	// 		// InterReloc::DropItemIntoWorld_Original(refr, handle, item, 0x7FFF, container, pa, pb, list);
	// 	}
	// 	// InterReloc::ExtraDataList_SetCount(extra, count);

	// 	// InterReloc::DropItemIntoWorld_Original(refr, handle, item, count, container, pa, pb, extra);
	// 	return handle;
	// }

}

// typedef void(bhkCharacterController_JumpSig)(RE::bhkCharacterController*, float);
// REL::Relocation<bhkCharacterController_JumpSig> JumpFunction;

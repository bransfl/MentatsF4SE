#pragma once
#include <cstdint>

namespace Internal::Fixes::TransferManyItems::DropManyItems
{
	void Install() noexcept;

	//uint32_t* DropItemIntoWorld_Hook(TESObjectREFR* refr, uint32_t* handle, TESBoundObject* item, SInt32 count, TESObjectREFR* container, NiPoint3* pa, NiPoint3* pb, ExtraDataList* extra);
}

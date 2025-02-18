#pragma once

namespace Internal::Fixes::TransferManyItems::DropManyItems
{
	void Install() noexcept;

	uint32_t* DropItemIntoWorld_Hook(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra);
}

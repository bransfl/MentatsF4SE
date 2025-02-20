#pragma once

namespace Internal::Fixes::TransferManyItems::DropManyItemsFix
{
	// installs the fix
	void Install() noexcept;

	// hooks
	uint32_t* Hook_DropItemIntoWorld_OG(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra);
	uint32_t* Hook_DropItemIntoWorld_NG(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra);
}

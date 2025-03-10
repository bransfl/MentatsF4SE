#pragma once

namespace Internal::Fixes
{
	class DropManyItemsFix
	{
	public:
		// installs the fix
		static void Install() noexcept;

	private:
		static void Fix();
		
		// og hook
		static uint32_t* Hook_DropItemIntoWorld_OG(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra);

		// ng hook
		static uint32_t* Hook_DropItemIntoWorld_NG(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra);

		// functions as ExtraDataList_SetCount, but doesnt req addresses and still works on all versions
		static void SetRefCount(RE::TESObjectREFR* a_itemRef, std::int16_t a_count);
	};
}

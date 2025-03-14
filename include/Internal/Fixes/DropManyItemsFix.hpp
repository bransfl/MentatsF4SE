#pragma once

namespace Internal::Fixes
{
	class DropManyItemsFix
	{
	public:
		/**
		 * @brief Initializes/registers the fix.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Patches relevant code.
		 */
		static void Fix();

		/**
		 * @brief Hook function for Last-Gen.
		 * @param a_refr
		 * @param a_handle
		 * @param a_item
		 * @param a_count
		 * @param a_container
		 * @param a_pa
		 * @param a_pb
		 * @param a_extra
		 * @return The a_handle parameter.
		 */
		static uint32_t* Hook_DropItemIntoWorld_OG(RE::TESObjectREFR* a_refr, uint32_t* a_handle, RE::TESBoundObject* a_item, int32_t a_count, RE::TESObjectREFR* a_container, RE::NiPoint3* a_pa, RE::NiPoint3* a_pb, RE::ExtraDataList* a_extra);

		/**
		 * @brief Hook function for Next-Gen.
		 * @param a_refr
		 * @param a_handle
		 * @param a_item
		 * @param a_count
		 * @param a_container
		 * @param a_pa
		 * @param a_pb
		 * @param a_extra
		 * @return The a_handle parameter.
		 */
		static uint32_t* Hook_DropItemIntoWorld_NG(RE::TESObjectREFR* a_refr, uint32_t* a_handle, RE::TESBoundObject* a_item, int32_t a_count, RE::TESObjectREFR* a_container, RE::NiPoint3* a_pa, RE::NiPoint3* a_pb, RE::ExtraDataList* a_extra);

		/**
		 * @brief Functions as ExtraDataList_SetCount, but doesnt req addresses and still works on all versions.
		 * @param a_itemRef The reference to modify.
		 * @param a_count The new count for the reference.
		 */
		static void SetRefCount(RE::TESObjectREFR* a_itemRef, std::int16_t a_count);
	};
}

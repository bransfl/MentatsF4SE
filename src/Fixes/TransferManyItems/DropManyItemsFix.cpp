#include "Internal/Fixes/TransferManyItems/DropManyItemsFix.hpp"
#include "Internal/Config/Config.hpp"
#include "REX/W32.hpp"
#include "f4se/GameApi.h"
#include <f4se_common/BranchTrampoline.h>
#include <f4se_common/Relocation.h>

// i know this is a mess. fuck this fix.
//
// note - this mod and the original mod both have the minor issue of
// dropping an individual item sometimes instead of a full 32,767 stack
// but it's better than voiding all of your stuff and having negative items
namespace Internal::Fixes::TransferManyItems::DropManyItemsFix
{
	typedef uint32_t* (*_DropItemIntoWorld)(RE::TESObjectREFR*, uint32_t*, RE::TESBoundObject*, int32_t, RE::TESObjectREFR*, RE::NiPoint3*, RE::NiPoint3*, RE::ExtraDataList*);

	// OG typedef/address
	//
	// NG typedef/address

	RelocAddr<_DropItemIntoWorld> DropItemIntoWorld_Original(0x003FA580);
	//
	RelocAddr<_DropItemIntoWorld> DropItemIntoWorld_Original_NG(0x004AD7E0);

	RelocPtr<void> DropItemIntoWorld_Dest(0x003F9BEB);
	//
	RelocPtr<void> DropItemIntoWorld_Dest_NG(0x003F9BEB);

	typedef RE::ExtraDataList* (*_ExtraDataList_ctor_OG)(void*);
	RelocAddr<_ExtraDataList_ctor_OG> ExtraDataList_ctor_OG(0x00080750);
	//
	typedef RE::ExtraDataList* (*_ExtraDataList_ctor_NG)(void*);
	RelocAddr<_ExtraDataList_ctor_NG> ExtraDataList_ctor_NG(0x0021E420);

	typedef void (*_ExtraDataList_dtor)(RE::ExtraDataList*);
	RelocAddr<_ExtraDataList_dtor> ExtraDataList_dtor(0x00080790);
	//
	typedef void (*_ExtraDataList_dtor_NG)(RE::ExtraDataList*);
	RelocAddr<_ExtraDataList_dtor_NG> ExtraDataList_dtor_NG(0x0021E460);

	typedef void (*_ExtraDataList_CopyList)(RE::ExtraDataList*, RE::ExtraDataList*);
	RelocAddr<_ExtraDataList_CopyList> ExtraDataList_CopyList(0x00082240);
	//
	typedef void (*_ExtraDataList_CopyList_NG)(RE::ExtraDataList*, RE::ExtraDataList*);
	RelocAddr<_ExtraDataList_CopyList_NG> ExtraDataList_CopyList_NG(0x002214C0);

	typedef void (*_ExtraDataList_SetCount)(RE::ExtraDataList*, int16_t);
	RelocAddr<_ExtraDataList_SetCount> ExtraDataList_SetCount(0x00086FB0);
	//
	typedef void (*_ExtraDataList_SetCount_NG)(RE::ExtraDataList*, int16_t);
	RelocAddr<_ExtraDataList_SetCount_NG> ExtraDataList_SetCount_NG(0x00226F00);

	void Install() noexcept
	{
		logger::info("Fix installing: DropManyItems.");

		if (!Config::bDropManyItemsFix.GetValue()) {
			logger::info("Fix aborted: DropManyItemsFix. Reason: Fix was disabled in ini file.");
			return;
		}
		if (std::filesystem::exists("Data/F4SE/Plugins/Drop7FFFPatch.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'Drop 7FFF Fix' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::warn("Fix aborted: DropManyItemsFix. Reason: Mod was installed: Drop7FFFPatch.dll.");
			return;
		}

		// i use gbranchtrampoline here since sylee used it and i'm clearly such a free thinker
		if (REL::Module::IsNG()) {
			// NG Patch
			if (!g_branchTrampoline.Create(14)) {
				logger::warn("DropManyItems couldn't create codegen buffer");
				return;
			}
			g_branchTrampoline.Write5Call(DropItemIntoWorld_Dest_NG.GetUIntPtr(), (uintptr_t)Hook_DropItemIntoWorld_NG);
		}
		else {
			// OG Patch
			if (!g_branchTrampoline.Create(14)) {
				logger::warn("DropManyItems couldn't create codegen buffer");
				return;
			}
			g_branchTrampoline.Write5Call(DropItemIntoWorld_Dest.GetUIntPtr(), (uintptr_t)Hook_DropItemIntoWorld_OG);
		}

		logger::info("Fix installed: DropManyItems.");
	}

	// og hook
	uint32_t* Hook_DropItemIntoWorld_OG(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra)
	{

		while (count >= 0x8000) {
			count -= 0x7FFF;

			RE::ExtraDataList* list = ExtraDataList_ctor_OG(Heap_Allocate(sizeof(RE::ExtraDataList)));

			REX::W32::InterlockedIncrement(&list->refCount);

			ExtraDataList_CopyList(extra, list);

			ExtraDataList_SetCount(list, 0x7FFF);

			DropItemIntoWorld_Original(refr, handle, item, 0x7FFF, container, pa, pb, list);

			if (REX::W32::InterlockedDecrement(&list->refCount) == 0) {
				ExtraDataList_dtor(list);
				Heap_Free(list);
			}
		}

		ExtraDataList_SetCount(extra, static_cast<int16_t>(count)); // seems to req direct conversion from 32 to 16

		DropItemIntoWorld_Original(refr, handle, item, count, container, pa, pb, extra);
		return handle;
	}

	// ng hook
	uint32_t* Hook_DropItemIntoWorld_NG(RE::TESObjectREFR* refr, uint32_t* handle, RE::TESBoundObject* item, int32_t count, RE::TESObjectREFR* container, RE::NiPoint3* pa, RE::NiPoint3* pb, RE::ExtraDataList* extra)
	{

		while (count >= 0x8000) {
			count -= 0x7FFF;

			RE::ExtraDataList* list = ExtraDataList_ctor_NG(Heap_Allocate(sizeof(RE::ExtraDataList)));

			REX::W32::InterlockedIncrement(&list->refCount);

			ExtraDataList_CopyList_NG(extra, list);

			ExtraDataList_SetCount_NG(list, 0x7FFF);

			DropItemIntoWorld_Original_NG(refr, handle, item, 0x7FFF, container, pa, pb, list);

			if (REX::W32::InterlockedDecrement(&list->refCount) == 0) {
				ExtraDataList_dtor_NG(list);
				Heap_Free(list);
			}
		}

		ExtraDataList_SetCount_NG(extra, static_cast<int16_t>(count)); // seems to req direct conversion from 32 to 16

		DropItemIntoWorld_Original_NG(refr, handle, item, count, container, pa, pb, extra);
		return handle;
	}
}

#include "Internal/Fixes/OutfitRedressFix.hpp"
#include "Internal/Config/Config.hpp"

namespace Internal::Fixes::OutfitRedressFix
{
	void Install() noexcept
	{
		logger::info("Fix installing: OutfitRedressFix"sv);

		if (!Config::bOutfitRedressFix.GetValue()) {
			logger::info("Fix aborted: OutfitRedressFix. Reason: Fix was disabled in ini file."sv);
			return;
		}
		if (REX::W32::GetModuleHandleW(L"Outfit-ReDress-Fix.dll")) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'Outfit Redress Fix' was detected. It is recommended that you disable this mod while using EngineFixesF4SE.\n");
			logger::info("Fix aborted: OutfitRedressFix. Reason: Mod was installed: Outfit-ReDress-Fix.dll."sv);
			return;
		}

		if (REL::Module::IsNG()) {
			// NG Patch - Don't have address for this yet.
			logger::info("Fix aborted: OutfitRedressFix. Reason: Game version was NG."sv);
			return;
		}
		else {
			// OG Patch
			auto& trampoline = F4SE::GetTrampoline();
			trampoline.create(20);
			RegisterHook(trampoline);
		}

		logger::info("Fix installed: OutfitRedressFix"sv);
	}

	typedef int64_t(Set3DUpdateFlagSig)(RE::AIProcess*, RE::RESET_3D_FLAGS);
	REL::Relocation<Set3DUpdateFlagSig> OriginalFunction_Set3DUpdateFlag;

	int64_t Hook_Set3DUpdateFlag(RE::AIProcess* a_process, RE::RESET_3D_FLAGS a_flags)
	{
		// TODO add check to prevent this from applying to danse OR actors in power armor
		// probably gonna have to check if currentpackage is danse's package in the helmet scene bro
		// if (a_process->currentPackage ==) {
		// 	return OriginalFunction_Set3DUpdateFlag(a_process, a_flags);
		// }

		a_flags = RE::RESET_3D_FLAGS::kModel;
		return OriginalFunction_Set3DUpdateFlag(a_process, a_flags);
	}

	void RegisterHook(F4SE::Trampoline& trampoline)
	{
		REL::Relocation<Set3DUpdateFlagSig> callLocation_OG{ REL::ID(323782), 0x29 };
		OriginalFunction_Set3DUpdateFlag = trampoline.write_call<5>(callLocation_OG.address(), &Hook_Set3DUpdateFlag);
	}
}

#include "Internal/Fixes/EncounterZoneResetFix.hpp"
#include "Internal/Config/Config.hpp"

// only needed for NG, Buffout4NG covers this for OG
// if alandtse updates Buffout4NG to include this for NG, this fix will be deprecated and removed
namespace Internal::Fixes::EncounterZoneResetFix
{
	void Install() noexcept
	{
		logger::info("EncounterZoneResetFix -> Fix installing..."sv);

		if (!Config::bEncounterZoneFix.GetValue()) {
			logger::info("Fix aborted: EncounterZoneResetFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		if (REX::W32::GetModuleHandleW(L"Buffout4.dll") && Config::bIsNG == false) {
			RE::ConsoleLog::GetSingleton()->PrintLine("EngineFixesF4SE - Mod 'Buffout 4 NG' was detected.\n");
			logger::warn("Fix aborted: EncounterZoneResetFix. Reason: Mod was installed: Buffout4.dll AND game version was OG."sv);
			return;
		}

		auto& cells = RE::CellAttachDetachEventSource::CellAttachDetachEventSourceSingleton::GetSingleton();
		cells.source.RegisterSink(Events::CellAttachDetachEventHandler::GetSingleton());

		logger::info("EncounterZoneResetFix -> Fix installed."sv);
	}
}

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

		// if buffout4ng is installed AND version is og AND buffout4ng's fix is enabled, abort
		if (REX::W32::GetModuleHandleW(L"Buffout4.dll") && !REL::Module::IsNG()) {
			logger::warn("Fix aborted: EncounterZoneResetFix. Reason: Mod was installed: Buffout4.dll AND game version was og"sv);
			return;
		}
	}

	auto& cells = RE::CellAttachDetachEventSource::CellAttachDetachEventSourceSingleton::GetSingleton();
	// cells.RegisterSink(Events::CellAttachDetachEventHandler::GetSingleton());

	// logger::info("EncounterZoneResetFix -> Fix installed."sv);
}

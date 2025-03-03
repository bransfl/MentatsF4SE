#include "Internal/Fixes/EncounterZoneResetFix.hpp"
#include "Internal/Config/Config.hpp"

// only needed for NG, Buffout4NG covers this for OG
// if alandtse updates Buffout4NG to include this for NG, this fix will be deprecated and removed
namespace Internal::Fixes
{
	void EncounterZoneResetFix::Install() noexcept
	{
		logger::info("EncounterZoneResetFix -> Fix installing..."sv);

		if (!Config::bEncounterZoneResetFix.GetValue()) {
			logger::info("Fix aborted: EncounterZoneResetFix. Reason: Fix was disabled in config file."sv);
			return;
		}

		// if buffout4ng is installed AND version is og, this is not needed
		if (!REL::Module::IsNG() && REX::W32::GetModuleHandleW(L"Buffout4.dll")) {
			logger::info("Fix aborted: EncounterZoneResetFix. Reason: Mod was installed: Buffout4.dll AND game version was og"sv);
			return;
		}

		auto& cells = RE::CellAttachDetachEventSource::CellAttachDetachEventSourceSingleton::GetSingleton();
		cells.source.RegisterSink(EncounterZoneResetFix::CellAttachDetachEventHandler::GetSingleton());

		logger::info("EncounterZoneResetFix -> Fix installed."sv);
	}
}

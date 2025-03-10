#include "Internal/Patches/PuddleCubemapsPatch.hpp"
#include "Internal/Config.hpp"

namespace Internal::Patches
{
	void PuddleCubemapsPatch::Install() noexcept
	{
		logger::info("PuddleCubemapsPatch -> Patch installing..."sv);

		if (!Config::bPuddleCubemapsPatch.GetValue()) {
			logger::info("Patch aborted: PuddleCubemapsPatch. Reason: Patch was disabled in config file."sv);
			return;
		}

		Fix();

		logger::info("PuddleCubemapsPatch -> Patch installed."sv);
	}

	void PuddleCubemapsPatch::Fix()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::warn("PuddleCubemapsPatch -> dataHandler was nullptr. Patch was not applied."sv);
			return;
		}

		auto worldspaces = dataHandler->GetFormArray<RE::TESWorldSpace>();
		RE::BSFixedString blank = RE::BSFixedString("");

		for (RE::TESWorldSpace* worldspace : worldspaces) {
			if (!worldspace) {
				logger::warn("PuddleCubemapsPatch -> Worldspace was nullptr."sv);
				continue;
			}

			worldspace->waterEnvMap.textureName = blank;
			logger::debug("PuddleCubemapsPatch -> Worldspace (FormID: {:08X}, EditorID: {}) - waterEnvMap was cleared."sv,
				worldspace->GetFormID(), worldspace->GetFormEditorID());
		}
	}
}

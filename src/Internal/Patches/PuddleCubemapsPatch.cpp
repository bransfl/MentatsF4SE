#include "Internal/Patches/PuddleCubemapsPatch.hpp"
#include "Internal/Config.hpp"

namespace Internal::Patches
{
	void PuddleCubemapsPatch::Install() noexcept
	{
		logger::info(FMT_STRING("PuddleCubemapsPatch -> Patch installing..."sv));

		if (!Config::bPuddleCubemapsPatch.GetValue()) {
			logger::info(FMT_STRING("Patch aborted: PuddleCubemapsPatch. Reason: Patch was disabled in config file."sv));
			return;
		}

		Fix();

		logger::info(FMT_STRING("PuddleCubemapsPatch -> Patch installed."sv));
	}

	void PuddleCubemapsPatch::Fix()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			return;
		}

		auto worldspaces = dataHandler->GetFormArray<RE::TESWorldSpace>();
		RE::BSFixedString blank = RE::BSFixedString("");

		for (RE::TESWorldSpace* worldspace : worldspaces) {
			if (!worldspace) {
				logger::warn(FMT_STRING("PuddleCubemapsPatch -> Worldspace was nullptr."));
				continue;
			}

			worldspace->waterEnvMap.textureName = blank;
			logger::info(FMT_STRING("PuddleCubemapsPatch -> Worldspace waterEnvMap was cleared: {}."), worldspace->GetFormEditorID());
		}
	}
}

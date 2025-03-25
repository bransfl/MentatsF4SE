#include "Internal/Patches/PuddleCubemapsPatch.hpp"
#include "Internal/Config.hpp"
#include "Internal/Utility.hpp"

namespace Internal::Patches
{
	void PuddleCubemapsPatch::Install() noexcept
	{
		logger::info("PuddleCubemapsPatch -> Patch installing..."sv);

		if (!Config::bPuddleCubemapsPatch.GetValue()) {
			logger::info("Patch aborted: PuddleCubemapsPatch. Reason: Patch was disabled in config file."sv);
			return;
		}

		Patch();

		logger::info("PuddleCubemapsPatch -> Patch installed."sv);
	}

	void PuddleCubemapsPatch::Patch()
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::warn("PuddleCubemapsPatch -> dataHandler was nullptr. Patch was not applied."sv);
			return;
		}

		auto worldspaces = dataHandler->GetFormArray<RE::TESWorldSpace>();
		if (worldspaces.size() == 0) {
			logger::warn("PuddleCubemapsPatch -> Worldspaces array size was 0. Patch was not applied."sv);
			return;
		}

		// textureName is a RE::BSFixedString type
		RE::BSFixedString blank = RE::BSFixedString("");

		for (RE::TESWorldSpace* worldspace : worldspaces) {
			if (!worldspace) {
				logger::warn("PuddleCubemapsPatch -> Worldspace was nullptr."sv);
				continue;
			}

			worldspace->waterEnvMap.textureName = blank;
			logger::info("PuddleCubemapsPatch -> Worldspace {} - waterEnvMap was cleared."sv,
				Utility::GetFormInfo(worldspace));
		}
	}
}

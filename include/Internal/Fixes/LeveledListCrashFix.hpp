#pragma once

namespace Internal::Fixes::LeveledListCrashFix
{
	void Install() noexcept;

	// returns the total amount of leveledlist entries
	int8_t GetListEntriesCount(RE::TESLeveledList* leveledList);

	// returns a vector of all of the forms in the leveledlist
	std::vector<RE::TESForm*> GetListEntries(RE::TESLeveledList* leveledList);

	namespace Hooks
	{
		struct ProtectLeveledItems
		{
			static inline void AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* arg2,
				unsigned short arg3, unsigned long long arg4,
				RE::TESForm* arg5);

			static inline REL::Relocation<decltype(&ProtectLeveledItems::AddForm)> _originalCall;

			static inline bool Install();
		};

		struct ProtectLeveledActors
		{
			static inline void AddForm(RE::TESLeveledList* a_this, RE::TESBoundObject* arg2,
				unsigned short arg3, unsigned long long arg4,
				RE::TESForm* arg5);

			static inline REL::Relocation<decltype(&ProtectLeveledItems::AddForm)> _originalCall;

			static inline bool Install();
		};
	}
	namespace Sanitizer
	{
		void Sanitize();
	}
}

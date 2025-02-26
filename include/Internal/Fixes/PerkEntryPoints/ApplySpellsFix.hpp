#pragma once

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpellsFix
{
	void Install() noexcept;

	// would be in BGSEntryPoint.hpp (or PerkEntries.hpp?)
	static RE::BGSEntryPointPerkEntry* GetEntryPoint(RE::BGSEntryPoint::ENTRY_POINT entryPoint);
	static RE::BGSEntryPointPerkEntry* GetEntryPoints();

	class ApplySpellsFix
	{
	public:
		static void ApplyCombatHitSpell(
			RE::BGSEntryPoint::ENTRY_POINT entryPoint,
			RE::Actor* perkOwner,
			...);

		static std::vector<void*> HandleEntryPoint(
			RE::BGSEntryPoint::ENTRY_POINT entryPoint,
			RE::Actor* perkOwner,
			std::vector<RE::TESForm*> conditionFilterArguments);
	};

	/* --------------------------------------------- */

	// hacky test/port from skse stuff to make stuff easier to read
	//
	// namespace Visitor
	// {
	// 	class RE::BGSPerkEntry;
	// 	class PerkEntryVisitor
	// 	{
	// 	public:
	// 		virtual RE::BSContainer::ForEachResult operator()(RE::BGSPerkEntry* perkEntry) = 0; // 0
	// 	};
	// 	static_assert(sizeof(PerkEntryVisitor) == 0x8);

	// 	class RE::Actor;
	// 	class RE::TESForm;

	// 	class HandleEntryPointVisitor
	// 		: public PerkEntryVisitor // 0
	// 	{
	// 	public:
	// 		// Override (PerkEntryVisitor)
	// 		virtual RE::BSContainer::ForEachResult operator()(RE::BGSPerkEntry* perkEntry) override; // 0

	// 		HandleEntryPointVisitor() = delete;
	// 		HandleEntryPointVisitor(const HandleEntryPointVisitor&) = delete;
	// 		HandleEntryPointVisitor(HandleEntryPointVisitor&&) = delete;

	// 		~HandleEntryPointVisitor() = default;

	// 		HandleEntryPointVisitor& operator=(const HandleEntryPointVisitor&) = delete;
	// 		HandleEntryPointVisitor& operator=(HandleEntryPointVisitor&&) = delete;

	// 		HandleEntryPointVisitor(
	// 			RE::BGSEntryPointFunctionData::FunctionType	entryPointFunctionType,
	// 			RE::TESForm** 								conditionFilterArguments,
	// 			void**										entryPointFunctionArguments,
	// 			RE::Actor*									perkOwner,
	// 			std::uint8_t								conditionFilterArgumentCount,
	// 			std::uint8_t								entryPointFunctionArgumentCount) {
	// 				this->entryPointFunctionType = entryPointFunctionType;
	// 				this->conditionFilterArguments = conditionFilterArguments;
	// 				this->entryPointFunctionArguments = entryPointFunctionArguments;
	// 				this->perkOwner = perkOwner;
	// 				this->conditionFilterArgumentCount = conditionFilterArgumentCount;
	// 				this->entryPointFunctionArgumentCount = entryPointFunctionArgumentCount;
	// 			}

	// 		// Member variables
	// 		RE::BGSEntryPointFunctionData::FunctionType entryPointFunctionType; // 8
	// 		RE::TESForm** conditionFilterArguments;								// 10
	// 		void** entryPointFunctionArguments;									// 18
	// 		RE::Actor* perkOwner;												// 20
	// 		uint8_t conditionFilterArgumentCount;								// 28
	// 		uint8_t entryPointFunctionArgumentCount;							// 29
	// 	};
	// }

}

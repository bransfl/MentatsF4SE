// #include "Internal/Fixes/ApplySpellsFix.hpp"

// #pragma warning(disable : 4505) // temp

// // fixes entrypoint ApplyCombatHitSpell so multiple spells can be applied at once
// namespace Internal::Fixes
// {
// 	typedef void(HandleEntryPointVisitorSig)(RE::Actor*, RE::BGSEntryPointFunctionData::FunctionType, RE::TESForm**, void**, uint8_t, uint8_t);
// 	REL::Relocation<HandleEntryPointVisitorSig> HandleEntryPointVisitor_OG(721434);
// 	REL::Relocation<HandleEntryPointVisitorSig> HandleEntryPointVisitor_NG(2766957);

// 	// typedef RE::BGSEntryPoint::ENTRY_POINT(GetEntryPointSig)(RE::BGSEntryPointPerkEntry* a_this);
// 	// REL::Relocation<GetEntryPointSig> GetEntryPoint(0x9E5AA3);
// 	// static RE::BGSEntryPoint::ENTRY_POINT GetEntryPoint(RE::BGSEntryPointPerkEntry* a_this)
// 	// {
// 	// 	return a_this->entryData.entryPoint;
// 	// }

// 	void ApplySpellsFix::Install() noexcept
// 	{
// 		logger::info("Fix installing: ApplySpellsFix."sv);

// 		if (!Config::bApplySpellsFix.GetValue()) {
// 			logger::info("Fix aborted: ApplySpellsFix. Reason: Fix was disabled in config."sv);
// 			return;
// 		}

// 		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
// 		// F4SE::AllocTrampoline(32);
// 		if (REL::Module::IsNG()) {
// 			// NG Patch
// 		}
// 		else {
// 			// OG Patch
// 		}

// 		logger::info("Fix installed: ApplySpellsFix."sv);
// 	}

// 	void ApplySpellsFix::ApplyCombatHitSpell(
// 		RE::BGSEntryPoint::ENTRY_POINT entryPoint,
// 		RE::Actor* perkOwner,
// 		...)
// 	{
// 		std::va_list varArgs;
// 		va_start(varArgs, perkOwner);
// 		auto* weapon = va_arg(varArgs, RE::TESObjectWEAP*);
// 		auto* target = va_arg(varArgs, RE::Actor*);
// 		// auto** result = va_arg(varArgs, RE::SpellItem**); // unused?
// 		va_end(varArgs);

// 		auto spellItemsList = ApplySpellsFix::HandleEntryPoint(entryPoint, perkOwner, std::vector<RE::TESForm*>{ perkOwner, weapon, target });

// 		for (auto* spellItem : spellItemsList) {
// 			if (spellItem) {
// 				// cast the spell on the target here
// 				// static_cast<Skyrim::SpellItem*>(combatHitSpellItem)->Apply(target, ApplySpells::castSpells_ ? perkOwner : target);
// 			}
// 		}
// 	}

// 	// char BGSEntryPoint::HandleEntryPoint(int aeEntryPoint, Actor *apPerkOwner, ...)
// 	// std::vector<void*> ApplySpellsFix::HandleEntryPoint(
// 	// 	RE::BGSEntryPoint::ENTRY_POINT entryPoint,
// 	// 	RE::Actor* perkOwner,
// 	// 	std::vector<RE::TESForm*> conditionFilterArguments)
// 	// {
// 	// 	uint8_t entryPointUnderlying = static_cast<uint8_t>(std::to_underlying(entryPoint));

// 	// 	// last enum type is (kSetFatigueToAPMult = 0x9D), so we can try 0x9E for skyrim's kTotal's equivalent

// 	// 	if (entryPointUnderlying < 0x9E) {
// 	// 		if (perkOwner && perkOwner->HasPerkEntries(entryPointUnderlying)) {

// 	// 			// conditionFilterArgumentCount is likely either (std::uint8_t numArgs, or std::uint8_t unk03 (guessed due to uint8_t)
// 	// 			// if (conditionFilterArguments.size() == ) {
// 	// 			// std::vector<void*> entryPointFunctionArgs = { std::numeric_limits<uint8_t>::max(), nullptr };
// 	// 			// // RE::HandleEntryPointVisitor handlePerkEntryVisitor = RE::HandleEntryPointVisitor(
// 	// 			// // 	perkOwner,
// 	// 			// // 	(GetEntryPoint(entryPoint)->functionData->GetType()),
// 	// 			// // 	(conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data()),
// 	// 			// // 	(entryPointFunctionArgs.empty() ? nullptr : entryPointFunctionArgs.data()),
// 	// 			// // 	static_cast<uint8_t>(conditionFilterArguments.size()),
// 	// 			// // 	static_cast<uint8_t>(entryPointFunctionArgs.size()));

// 	// 			// // RE::HandleEntryPointVisitor visitor = RE::HandleEntryPointVisitor(
// 	// 			// // 	perkOwner,

// 	// 			// // );
// 	// 			// // typedef void(HandleEntryPointVisitorSig)(RE::Actor*, RE::BGSEntryPointFunctionData::FunctionType, RE::BGSObjectInstance**, void**, uint8_t, uint8_t);

// 	// 			// // perkOwner->ForEachPerkEntry(entryPointUnderlying, HandleEntryPointVisitor_OG(
// 	// 			// // 											perkOwner,
// 	// 			// // 											(GetEntryPoint(entryPoint)->functionData->GetType()),
// 	// 			// // 											(conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data()),
// 	// 			// // 											(entryPointFunctionArgs.empty() ? nullptr : entryPointFunctionArgs.data()),
// 	// 			// // 											static_cast<uint8_t>(conditionFilterArguments.size()),
// 	// 			// // 											static_cast<uint8_t>(entryPointFunctionArgs.size())));

// 	// 			// std::erase(entryPointFunctionArgs, nullptr);

// 	// 			// return entryPointFunctionArgs;
// 	// 			//}
// 	// 		}
// 	// 	}

// 	// 	return {};
// 	// }
// }

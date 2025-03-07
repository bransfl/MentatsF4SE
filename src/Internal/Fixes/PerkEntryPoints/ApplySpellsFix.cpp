#include "Internal/Fixes/PerkEntryPoints/ApplySpellsFix.hpp"
#include "Internal/Config/Config.hpp"

// fixes entrypoint ApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints
{
	typedef void(HandleEntryPointVisitorSig)(RE::Actor*, RE::BGSEntryPointFunctionData::FunctionType, RE::TESForm**, void**, uint8_t, uint8_t);
	REL::Relocation<HandleEntryPointVisitorSig> HandleEntryPointVisitor_OG(721434);
	REL::Relocation<HandleEntryPointVisitorSig> HandleEntryPointVisitor_NG(2766957);

	// address might be 699291 (0x9E5AA3) - its not
	// these dont work yet
	// static RE::BGSEntryPointPerkEntry* GetEntryPoint(RE::BGSEntryPoint::ENTRY_POINT entryPoint)
	// {
	// 	return std::addressof(GetEntryPoints()[std::to_underlying(entryPoint)]);
	// }

	// static RE::BGSEntryPointPerkEntry* GetEntryPoints()
	// {
	// 	// inline constexpr REL::RelocationID BGSEntryPointPerkEntry{ 928907, 2767025 };
	// 	if (REL::Module::IsNG()) {
	// 		REL::Relocation<uintptr_t> BGSEntryPointPerkEntry_Location{ REL::ID(2767025) };
	// 		auto* singleton{ reinterpret_cast<RE::BGSEntryPointPerkEntry*>(BGSEntryPointPerkEntry_Location.address()) };
	// 		return singleton;
	// 	}

	// 	else {

	// 		REL::Relocation<uintptr_t> BGSEntryPointPerkEntry_Location{ REL::ID(928907) };
	// 		auto* singleton{ reinterpret_cast<RE::BGSEntryPointPerkEntry*>(BGSEntryPointPerkEntry_Location.address()) };
	// 		return singleton;
	// 	}
	// }

	void ApplySpellsFix::Install() noexcept
	{
		logger::info(FMT_STRING("Fix installing: ApplySpellsFix."sv));

		if (!Config::bApplySpellsFix.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: ApplySpellsFix. Reason: Fix was disabled in config file."sv));
			return;
		}

		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();
		// F4SE::AllocTrampoline(32); maybe?
		if (REL::Module::IsNG()) {
			// NG Patch
		}
		else {
			// OG Patch
		}

		logger::info(FMT_STRING("Fix installed: ApplySpellsFix."sv));
	}

	void ApplySpellsFix::ApplyCombatHitSpell(
		RE::BGSEntryPoint::ENTRY_POINT entryPoint,
		RE::Actor* perkOwner,
		...)
	{
		std::va_list varArgs;
		va_start(varArgs, perkOwner);
		auto* weapon = va_arg(varArgs, RE::TESObjectWEAP*);
		auto* target = va_arg(varArgs, RE::Actor*);
		// auto** result = va_arg(varArgs, RE::SpellItem**); // unused?
		va_end(varArgs);

		auto spellItemsList = ApplySpellsFix::HandleEntryPoint(entryPoint, perkOwner, std::vector<RE::TESForm*>{ perkOwner, weapon, target });

		for (auto* spellItem : spellItemsList) {
			if (spellItem) {
				// cast here
				// static_cast<Skyrim::SpellItem*>(combatHitSpellItem)->Apply(target, ApplySpells::castSpells_ ? perkOwner : target);
				// static_cast<RE::SpellItem*>(spellItem)->Cast(perkOwner, target, perkOwner, /* add vm here? */);
			}
		}
		// bool Cast(TESObjectREFR* caster, TESObjectREFR* target, Actor* anyActor, BSScript::IVirtualMachine* vm)
		// {
		// 	using func_t = decltype(&SpellItem::Cast);
		// 	static REL::Relocation<func_t> func{ REL::ID(1511987) };
		// 	return func(this, caster, target, anyActor, vm);
		// }
	}

	std::vector<void*> ApplySpellsFix::HandleEntryPoint(
		RE::BGSEntryPoint::ENTRY_POINT entryPoint,
		RE::Actor* perkOwner,
		std::vector<RE::TESForm*> conditionFilterArguments)
	{
		uint8_t entryPointUnderlying = static_cast<uint8_t>(std::to_underlying(entryPoint));

		// last enum type is (kSetFatigueToAPMult = 0x9D), so we can try 0x9E for skyrim's kTotal's equivalent

		if (entryPointUnderlying < 0x9E) {
			if (perkOwner && perkOwner->HasPerkEntries(entryPointUnderlying)) {

				// conditionFilterArgumentCount is likely either (std::uint8_t numArgs, or std::uint8_t unk03 (guessed due to uint8_t)
				// if (conditionFilterArguments.size() == GetEntryPoint(entryPoint)->entryData.numArgs) {
				// std::vector<void*> entryPointFunctionArgs = { std::numeric_limits<uint8_t>::max(), nullptr };
				// // RE::HandleEntryPointVisitor handlePerkEntryVisitor = RE::HandleEntryPointVisitor(
				// // 	perkOwner,
				// // 	(GetEntryPoint(entryPoint)->functionData->GetType()),
				// // 	(conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data()),
				// // 	(entryPointFunctionArgs.empty() ? nullptr : entryPointFunctionArgs.data()),
				// // 	static_cast<uint8_t>(conditionFilterArguments.size()),
				// // 	static_cast<uint8_t>(entryPointFunctionArgs.size()));

				// // RE::HandleEntryPointVisitor visitor = RE::HandleEntryPointVisitor(
				// // 	perkOwner,

				// // );
				// // typedef void(HandleEntryPointVisitorSig)(RE::Actor*, RE::BGSEntryPointFunctionData::FunctionType, RE::BGSObjectInstance**, void**, uint8_t, uint8_t);

				// // perkOwner->ForEachPerkEntry(entryPointUnderlying, HandleEntryPointVisitor_OG(
				// // 											perkOwner,
				// // 											(GetEntryPoint(entryPoint)->functionData->GetType()),
				// // 											(conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data()),
				// // 											(entryPointFunctionArgs.empty() ? nullptr : entryPointFunctionArgs.data()),
				// // 											static_cast<uint8_t>(conditionFilterArguments.size()),
				// // 											static_cast<uint8_t>(entryPointFunctionArgs.size())));

				// std::erase(entryPointFunctionArgs, nullptr);

				// return entryPointFunctionArgs;
				//}
			}
		}

		return {};
	}
}

// virtual void ForEachPerkEntry(std::uint8_t a_entryPoint, PerkEntryVisitor& a_visitor) const;
// // bp42s added start
// class PerkEntryVisitor
// {
// 	// add
// 	virtual BSContainer::ForEachResult operator()(BGSPerkEntry* a_entry) = 0; // 00
// };
// static_assert(sizeof(PerkEntryVisitor) == 0x8);

// class HandleEntryPointVisitor : public PerkEntryVisitor // 00
// {
// 	Actor* perkOwner;												// 08
// 	RE::BGSEntryPointFunctionData::FunctionType eFunctionType; // 10
// 	BGSObjectInstance** filterForms;								// 18
// 	void** functionArguments;										// 20
// 	uint8_t parameterNum;											// 28
// 	uint8_t argumentNum;											// 29
// };
// static_assert(sizeof(HandleEntryPointVisitor) == 0x30);
// // bp42s added end

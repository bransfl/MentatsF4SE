#include "Internal/Papyrus.hpp"

#pragma warning(push)
#pragma warning(disable : 4100) // unreferenced formal parameters


namespace Internal
{
	bool Papyrus::IsMentatsInstalled(DEFAULT_SCRIPT_PARAMS) noexcept
	{
		return true;
	}

	bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* a_vm) noexcept
	{
		a_vm->BindNativeMethod(SCRIPT_NAME, "IsMentatsInstalled"sv, IsMentatsInstalled, true);

		return true;
	}

	bool Papyrus::Callback(RE::BSScript::IVirtualMachine* a_vm) noexcept
	{
		if (!RegisterFunctions(a_vm)) {
			logger::error("Failed to register papyrus functions"sv);
			return false;
		}

		logger::info("Papyrus functions successfully registered"sv);
		return true;
	}
}

#pragma warning(pop)

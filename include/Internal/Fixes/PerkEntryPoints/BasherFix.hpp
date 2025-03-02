#pragma once

// fixes entrypoint ModOutgoingLimbBashDamage so it actually works
// note - this will likely require RE, and wont be made until RE is done.
namespace Internal::Fixes::PerkEntryPoints::BasherFix
{
	// installs the fix
	void Install() noexcept;

	// RE::BGSEntryPoint::ENTRY_POINT::kModBashingDamage = 0x1A,
}

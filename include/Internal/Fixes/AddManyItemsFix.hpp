#pragma once

namespace Internal::Fixes
{
	class AddManyItemsFix
	{
	public:
		// installs the fix
		static void Install() noexcept;

	private:
		static void Fix();
	};
}

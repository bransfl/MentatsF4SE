#pragma once

namespace Internal
{
	class Messaging
	{
	public:
		static void RegisterCallback();

		static void Callback(F4SE::MessagingInterface::Message* a_msg) noexcept;
	};
}

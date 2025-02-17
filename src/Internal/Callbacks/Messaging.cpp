#include "Internal/Fixes/CellAttachDetachEvent.hpp"

namespace Internal::Messaging
{
	// handles various F4SE callback events
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		logger::debug("Messaging: Received message of type: {}"sv, a_msg->type);

		// uncomment event cases as needed
		switch (a_msg->type) {
			// case F4SE::MessagingInterface::kPostLoad: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kPostPostLoad: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kPreLoadGame: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kPostLoadGame: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kPreSaveGame: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kPostSaveGame: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kDeleteGame: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kInputLoaded: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kNewGame: {
			// 	break;
			// }
			// case F4SE::MessagingInterface::kGameLoaded: {
			// 	break;
			// }
			case F4SE::MessagingInterface::kGameDataReady: {
				Internal::Fixes::CellAttachDetachEvent::Install();
				break;
			}
			default: {
				break;
			}
		}
	}
}

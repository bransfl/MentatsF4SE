namespace Internal::Messaging
{
	// handles various F4SE events
	void Callback(F4SE::MessagingInterface::Message* a_msg)
	{
		logger::debug("Messaging: Received message of type: {}"sv, a_msg->type);

		switch (a_msg->type) {
			case F4SE::MessagingInterface::kGameDataReady: {
				break;
			}
			case F4SE::MessagingInterface::kGameLoaded: {
				break;
			}
			case F4SE::MessagingInterface::kNewGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostLoadGame: {
				break;
			}
			case F4SE::MessagingInterface::kPreSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kPostSaveGame: {
				break;
			}
			case F4SE::MessagingInterface::kDeleteGame: {
				break;
			}
		}
	}
}

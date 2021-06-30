#pragma once

#include <cstdint>

enum class PROTOCOL : uint16_t
{
	// Error 0~999
	TP_ERROR = 0,

	// C2S Req 1000~1999
	REQ_LOGIN = 1000,
	REQ_MOVE = 1001,
	REQ_LOCATION_SYNC = 1002,
	REQ_ROUND_TRIP_TIME = 1003,
	REQ_INPUT_ACTION = 1004,

	// S2C Res 2000~2999
	RES_LOGIN = 2000,
	RES_ROUND_TRIP_TIME = 2001,

	// S2C Broadcast 3000~3999
	BCAST_ENTER_GAME_ROOM = 3000,
	BCAST_EXIT_GAME_ROOM = 3001,
	BCAST_MOVE = 3002,
	BCAST_LOCATION_SYNC = 3003,
	BCAST_INPUT_ACTION = 3004,

	END_OF_PACKET = 20000,
};
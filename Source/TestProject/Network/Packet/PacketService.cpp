#include "PacketService.h"
#include "../../Object/ObjUser.h"
#include "Packet.h"

void PacketService::Process(const Packet& packet)
{
	auto header = packet.GetHeader();
	switch (header)
	{
	case PROTOCOL::TP_ERROR:
	{
		auto req = flatbuffers::GetRoot<TB_Error>(packet.GetBody());
		wchar_t wMessage[MAX_BUFF_SIZE];
		TPUtil::GetInstance().MultiByteToWChar(wMessage, MAX_BUFF_SIZE, req->Message()->c_str());
		recvCallError(FString(wMessage));
		break;
	}
	case PROTOCOL::RES_LOGIN:
	{
		auto req = flatbuffers::GetRoot<TB_ResLogin>(packet.GetBody());
		auto objUserList = req->ObjUserList();
		if (objUserList && objUserList->size() > 0)
		{
			TArray<UObjUser*> resultObjList;
			for (auto it = objUserList->begin(); it != objUserList->end(); ++it)
			{
				auto newObjUser = UObjUser::Create(*(*it));
				resultObjList.Add(newObjUser);
			}
			recvCallResLogin(resultObjList);
		}	
		break;
	}
	case PROTOCOL::RES_ROUND_TRIP_TIME:
	{
		auto req = flatbuffers::GetRoot<TB_ResRoundTripTime>(packet.GetBody());
		auto currentTimeMs = req->CurrentTimeMs();
		auto roundTripTimeMs = req->RoundTripTimeMs();
		recvCallResRoundTripTime(currentTimeMs, roundTripTimeMs);
		break;
	}
	case PROTOCOL::BCAST_ENTER_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_BcastEnterGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		if (objUser)
		{
			auto newObjUser = UObjUser::Create(*objUser);
			recvCallBcastEnterGameRoom(newObjUser);
		}
		break;
	}
	case PROTOCOL::BCAST_EXIT_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_BcastExitGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		if (objUser)
		{
			wchar_t wUserId[SIZE_USER_USER_ID];
			TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, objUser->UserId()->c_str());
			recvCallBcastExitGameRoom(FString(wUserId));
		}
		break;
	}
	case PROTOCOL::BCAST_MOVE:
	{
		auto req = flatbuffers::GetRoot<TB_BcastMove>(packet.GetBody());
		recvCallBcastMove(FBcastMove(*req));
		break;
	}
	case PROTOCOL::BCAST_LOCATION_SYNC:
	{
		auto req = flatbuffers::GetRoot<TB_BcastLocationSync>(packet.GetBody());
		recvCallBcastLocationSync(FBcastLocationSync(*req));
		break;
	}
	case PROTOCOL::BCAST_ACTION:
	{
		auto req = flatbuffers::GetRoot<TB_BcastAction>(packet.GetBody());
		recvCallBcastAction(FBcastAction(*req));
		break;
	}
	case PROTOCOL::BCAST_HIT:
	{
		auto req = flatbuffers::GetRoot<TB_BcastHit>(packet.GetBody());
		auto objUserList = req->ObjUserList();
		if (objUserList && objUserList->size() > 0)
		{
			TArray<UObjUser*> resultObjList;
			for (auto it = objUserList->begin(); it != objUserList->end(); ++it)
			{
				auto newObjUser = UObjUser::Create(*(*it));
				resultObjList.Add(newObjUser);
			}
			recvCallBcastHit(resultObjList);
		}
		break;
	}
	case PROTOCOL::BCAST_ROTATION_SYNC:
	{
		auto req = flatbuffers::GetRoot<TB_BcastRotationSync>(packet.GetBody());
		recvCallBcastRotationSync(FBcastRotationSync(*req));
		break;
	}
	default:
		break;
	}
}

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
				wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
				TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, it->UserId()->c_str());
				TPUtil::GetInstance().MultiByteToWChar(wPassword, SIZE_USER_PASSWORD, it->Password()->c_str());
				auto newObjUser = UObjUser::Create(wUserId, wPassword);
				auto newCompUserLocation = UCompUserLocation::Create(it->UserLocation()->Location()->x(), it->UserLocation()->Location()->y(), it->UserLocation()->Location()->z());
				newObjUser->SetCompUserLocation(newCompUserLocation);
				resultObjList.Add(newObjUser);
			}
			recvCallResLogin(resultObjList);
		}	
		break;
	}
	case PROTOCOL::BCAST_ENTER_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_BcastEnterGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		if (objUser)
		{
			wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
			TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, objUser->UserId()->c_str());
			TPUtil::GetInstance().MultiByteToWChar(wPassword, SIZE_USER_PASSWORD, objUser->Password()->c_str());
			auto newObjUser = UObjUser::Create(wUserId, wPassword);
			auto newCompUserLocation = UCompUserLocation::Create(objUser->UserLocation()->Location()->x(), objUser->UserLocation()->Location()->y(), objUser->UserLocation()->Location()->z());
			newObjUser->SetCompUserLocation(newCompUserLocation);
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
		auto userId = req->UserId()->c_str();
		auto inputMove = req->InputMove();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, userId);
	
		recvCallBcastMove(FBcastMove(*req));
		break;
	}
	case PROTOCOL::BCAST_LOCATION_SYNC:
	{
		auto req = flatbuffers::GetRoot<TB_BcastLocationSync>(packet.GetBody());
		auto userId = req->UserId()->c_str();
		auto location = req->Location();

		wchar_t wUserId[SIZE_USER_USER_ID];
		TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, userId);

		FVector fLocation(location->x(), location->y(), location->z());

		recvCallBcastLocationSync(FString(wUserId), fLocation);
		break;
	}
	default:
		break;
	}
}

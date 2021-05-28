#include "PacketService.h"
#include "ObjUser.h"

void PacketService::Process(const Packet& packet)
{
	auto header = packet.GetHeader();
	switch (header)
	{
	case PROTOCOL::TP_ERROR:
	{
		auto req = flatbuffers::GetRoot<TB_Error>(packet.GetBody());
		wchar_t wMessage[BUFSIZE];
		TPUtil::GetInstance().MultiByteToWChar(wMessage, BUFSIZE, req->Message()->c_str());
		recvCallError(FString(wMessage));
		break;
	}
	case PROTOCOL::GAME_ROOM_OBJ:
	{
		auto req = flatbuffers::GetRoot<TB_GameRoomObj>(packet.GetBody());
		auto objUserList = req->ObjUserList();
		if (objUserList && objUserList->size() > 0)
		{
			TArray<UObjUser*> resultObjList;
			for (auto it = objUserList->begin(); it != objUserList->end(); ++it)
			{
				wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
				TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, it->UserId()->c_str());
				TPUtil::GetInstance().MultiByteToWChar(wPassword, SIZE_USER_PASSWORD, it->Password()->c_str());
				UE_LOG(LogTemp, Log, TEXT("UserId:%s Password:%s"), *FString(wUserId), *FString(wPassword));
				UE_LOG(LogTemp, Log, TEXT("Location:%f,%f,%f"), it->UserLocation()->Location()->x(), it->UserLocation()->Location()->y(), it->UserLocation()->Location()->z());
				auto newObjUser = UObjUser::Create(wUserId, wPassword);
				auto newCompUserLocation = UCompUserLocation::Create(it->UserLocation()->Location()->x(), it->UserLocation()->Location()->y(), it->UserLocation()->Location()->z());
				newObjUser->SetCompUserLocation(newCompUserLocation);
				resultObjList.Add(newObjUser);
			}
			recvCallGameRoomObj(resultObjList);
		}	
		break;
	}
	case PROTOCOL::ENTER_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_EnterGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		if (objUser)
		{
			wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
			TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, objUser->UserId()->c_str());
			TPUtil::GetInstance().MultiByteToWChar(wPassword, SIZE_USER_PASSWORD, objUser->Password()->c_str());
			UE_LOG(LogTemp, Log, TEXT("UserId:%s Password:%s"), *FString(wUserId), *FString(wPassword));
			UE_LOG(LogTemp, Log, TEXT("Location:%f,%f,%f"), objUser->UserLocation()->Location()->x(), objUser->UserLocation()->Location()->y(), objUser->UserLocation()->Location()->z());
			auto newObjUser = UObjUser::Create(wUserId, wPassword);
			auto newCompUserLocation = UCompUserLocation::Create(objUser->UserLocation()->Location()->x(), objUser->UserLocation()->Location()->y(), objUser->UserLocation()->Location()->z());
			newObjUser->SetCompUserLocation(newCompUserLocation);
			recvCallEnterGameRoom(newObjUser);
		}
		break;
	}
	case PROTOCOL::EXIT_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_ExitGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		if (objUser)
		{
			wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
			TPUtil::GetInstance().MultiByteToWChar(wUserId, SIZE_USER_USER_ID, objUser->UserId()->c_str());
			TPUtil::GetInstance().MultiByteToWChar(wPassword, SIZE_USER_PASSWORD, objUser->Password()->c_str());
			UE_LOG(LogTemp, Log, TEXT("UserId:%s Password:%s"), *FString(wUserId), *FString(wPassword));
			UE_LOG(LogTemp, Log, TEXT("Location:%f,%f,%f"), objUser->UserLocation()->Location()->x(), objUser->UserLocation()->Location()->y(), objUser->UserLocation()->Location()->z());
			recvCallExitGameRoom(FString(wUserId));
		}
		break;
	}
	default:
		break;
	}
}

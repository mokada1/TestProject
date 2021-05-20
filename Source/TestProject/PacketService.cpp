#include "PacketService.h"
#include "TP_generated.h"

#include <iostream>

using namespace std;

void PacketService::Process(const Packet& packet)
{
	auto header = packet.GetHeader();
	switch (header)
	{
	case PROTOCOL::TP_ERROR:
	{
		auto req = flatbuffers::GetRoot<TB_Error>(packet.GetBody());
		UE_LOG(LogTemp, Error, TEXT("Message:%s"), *FString(req->Message()->c_str()));
		break;
	}
	case PROTOCOL::GAME_ROOM_OBJ:
	{
		auto req = flatbuffers::GetRoot<TB_GameRoomObj>(packet.GetBody());
		auto objUserList = req->ObjUserList();
		for (auto it = objUserList->begin(); it != objUserList->end(); ++it)
		{
			UE_LOG(LogTemp, Log, TEXT("UserId:%s Password:%s"), *FString(it->UserId()->c_str()), *FString(it->Password()->c_str()));
			UE_LOG(LogTemp, Log, TEXT("Location:%f,%f,%f"), it->UserLocation()->Location()->x(), it->UserLocation()->Location()->y(), it->UserLocation()->Location()->z());
		}
		break;
	}
	case PROTOCOL::ENTER_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_EnterGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		UE_LOG(LogTemp, Log, TEXT("UserId:%s Password:%s"), *FString(objUser->UserId()->c_str()), *FString(objUser->Password()->c_str()));
		UE_LOG(LogTemp, Log, TEXT("Location:%f,%f,%f"), objUser->UserLocation()->Location()->x(), objUser->UserLocation()->Location()->y(), objUser->UserLocation()->Location()->z());
		break;
	}
	case PROTOCOL::EXIT_GAME_ROOM:
	{
		auto req = flatbuffers::GetRoot<TB_ExitGameRoom>(packet.GetBody());
		auto objUser = req->ObjUser();
		UE_LOG(LogTemp, Log, TEXT("UserId:%s Password:%s"), *FString(objUser->UserId()->c_str()), *FString(objUser->Password()->c_str()));
		UE_LOG(LogTemp, Log, TEXT("Location:%f,%f,%f"), objUser->UserLocation()->Location()->x(), objUser->UserLocation()->Location()->y(), objUser->UserLocation()->Location()->z());
		break;
	}
	default:
		break;
	}
}
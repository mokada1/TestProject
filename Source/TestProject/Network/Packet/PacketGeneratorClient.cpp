#include "PacketGeneratorClient.h"
#include "../../Util/TPUtil.h"

Packet PacketGeneratorClient::CreateReqLogin(const string& userId, const string& password)
{
	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(userId);
	auto offsetPassword = fbb.CreateString(password);

	fbb.Finish(CreateTB_ReqLogin(fbb, offsetUserId, offsetPassword));

	return CreatePacket(PROTOCOL::REQ_LOGIN, fbb, nullptr);
}

Packet PacketGeneratorClient::CreateReqMove(FBcastMove& bcastMove)
{
	flatbuffers::FlatBufferBuilder fbb;

	auto offsetOperation = bcastMove.GetOffsetOperation();
	auto offsetInputMove = bcastMove.inputMove.Serialize(fbb);

	fbb.Finish(CreateTB_ReqMove(fbb, offsetOperation, offsetInputMove));

	return CreatePacket(PROTOCOL::REQ_MOVE, fbb, nullptr);
}

Packet PacketGeneratorClient::CreateReqLocationSync(const FVector& location)
{
	flatbuffers::FlatBufferBuilder fbb;

	ST_Vec3 stLocation(location.X, location.Y, location.Z);

	fbb.Finish(CreateTB_ReqLocationSync(fbb, &stLocation));

	return CreatePacket(PROTOCOL::REQ_LOCATION_SYNC, fbb, nullptr);
}

Packet PacketGeneratorClient::CreateReqRoundTripTime()
{
	flatbuffers::FlatBufferBuilder fbb;

	auto currentTimeMs = static_cast<int64_t>(TPUtil::GetInstance().TimeSinceEpochMs());

	fbb.Finish(CreateTB_ReqRoundTripTime(fbb, currentTimeMs));

	return CreatePacket(PROTOCOL::REQ_ROUND_TRIP_TIME, fbb, nullptr);
}

Packet PacketGeneratorClient::CreateReqAction(FBcastAction& bcastAction)
{
	flatbuffers::FlatBufferBuilder fbb;

	auto offsetOperation = bcastAction.GetOffsetOperation();
	auto offsetInputAction = bcastAction.inputAction.Serialize(fbb);

	fbb.Finish(CreateTB_ReqAction(fbb, offsetOperation, offsetInputAction));

	return CreatePacket(PROTOCOL::REQ_ACTION, fbb, nullptr);
}

Packet PacketGeneratorClient::CreateReqDamage()
{
	flatbuffers::FlatBufferBuilder fbb;

	fbb.Finish(CreateTB_ReqDamage(fbb));

	return CreatePacket(PROTOCOL::REQ_DAMAGE, fbb, nullptr);
}

Packet PacketGeneratorClient::CreateReqRotationSync(const FVector& rotation)
{
	flatbuffers::FlatBufferBuilder fbb;

	ST_Vec3 stRotation(rotation.X, rotation.Y, rotation.Z);

	fbb.Finish(CreateTB_ReqRotationSync(fbb, &stRotation));

	return CreatePacket(PROTOCOL::REQ_ROTATION_SYNC, fbb, nullptr);
}
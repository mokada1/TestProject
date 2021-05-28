#include "CompUserLocation.h"

FVector UCompUserLocation::BP_GetLocation() const
{
	return FVector(location.x, location.y, location.z);
}

bool UCompUserLocation::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserLocation> UCompUserLocation::SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const
{
	ST_Vec3 st_location(location.x, location.y, location.z);
	TB_CompUserLocationBuilder builder(_fbb);
	builder.add_Location(&st_location);
	return builder.Finish();
}

UCompUserLocation* UCompUserLocation::Create(const float _x, const float _y, const float _z)
{
	auto comp = NewObject<UCompUserLocation>();
	comp->Init(_x, _y, _z);
	return comp;
}

void UCompUserLocation::Init()
{
	isValid = false;
	location = { 0.f, 0.f, 0.f };
}

void UCompUserLocation::Init(const float _x, const float _y, const float _z)
{
	isValid = true;
	location = { _x, _y, _z };
}

void UCompUserLocation::Init(const double _x, const double _y, const double _z)
{
	isValid = true;
	location = { static_cast<float>(_x), static_cast<float>(_y), static_cast<float>(_z) };
}

void UCompUserLocation::Init(const Vector3 _location)
{
	isValid = true;
	location = _location;
}

UCompUserLocation::~UCompUserLocation()
{
}

Vector3 UCompUserLocation::GetLocation() const
{
	return location;
}
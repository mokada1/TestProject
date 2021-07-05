#include "CompUserTransform.h"

FVector UCompUserTransform::BP_GetLocation() const
{
	return FVector(location.x, location.y, location.z);
}

FVector UCompUserTransform::BP_GetRotation() const
{
	return FVector(rotation.x, rotation.y, rotation.z);
}

bool UCompUserTransform::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserTransform> UCompUserTransform::SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const
{
	ST_Vec3 st_location(location.x, location.y, location.z);
	ST_Vec3 st_rotation(rotation.x, rotation.y, rotation.z);

	TB_CompUserTransformBuilder builder(_fbb);
	builder.add_Location(&st_location);
	builder.add_Rotation(&st_rotation);
	return builder.Finish();
}

UCompUserTransform* UCompUserTransform::Create(const Vector3 _location, const Vector3 _rotation)
{
	auto comp = NewObject<UCompUserTransform>();
	comp->Init(_location, _rotation);
	return comp;
}

UCompUserTransform::~UCompUserTransform()
{
}

void UCompUserTransform::Init()
{
	isValid = false;
	location = { 0.f, 0.f, 0.f };
	rotation = { 0.f, 0.f, 0.f };
}

void UCompUserTransform::Init(const Vector3 _location, const Vector3 _rotation)
{
	isValid = true;
	location = _location;
	rotation = _rotation;
}

Vector3 UCompUserTransform::GetLocation() const
{
	return location;
}

Vector3 UCompUserTransform::GetRotation() const
{
	return rotation;
}

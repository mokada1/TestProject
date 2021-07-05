#pragma once

#include "../../TP_generated.h"
#include "InputAction.generated.h"

USTRUCT(BlueprintType)
struct FInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector location;

	UPROPERTY(BlueprintReadWrite)
	FVector rotation;

	FInputAction()
	{
		location = FVector(0.f, 0.f, 0.f);
		rotation = FVector(0.f, 0.f, 0.f);
	}
	FInputAction(const TB_InputAction& InputAction)
	{
		location = FVector(InputAction.Location()->x(),
			InputAction.Location()->y(),
			InputAction.Location()->z()
		);
		rotation = FVector(InputAction.Rotation()->x(),
			InputAction.Rotation()->y(),
			InputAction.Rotation()->z()
		);
	}

	flatbuffers::Offset<TB_InputAction> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		ST_Vec3 stLocation(location.X, location.Y, location.Z);
		ST_Vec3 stRotation(rotation.X, rotation.Y, rotation.Z);

		TB_InputActionBuilder builder(_fbb);
		builder.add_Location(&stLocation);
		builder.add_Rotation(&stRotation);
		return builder.Finish();
	}
};
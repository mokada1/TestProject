#pragma once

#include "../../TP_generated.h"
#include "../../Util/TPUtil.h"
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

	UPROPERTY(BlueprintReadWrite)
	FString comboSectionName;

	UPROPERTY(BlueprintReadWrite)
	bool isCombatPosture;

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
		if (InputAction.ComboSectionName())
		{
			comboSectionName = FString(InputAction.ComboSectionName()->c_str());
		}
		isCombatPosture = InputAction.IsCombatPosture();
	}

	flatbuffers::Offset<TB_InputAction> Serialize(flatbuffers::FlatBufferBuilder& _fbb)
	{
		ST_Vec3 stLocation(location.X, location.Y, location.Z);
		ST_Vec3 stRotation(rotation.X, rotation.Y, rotation.Z);

		flatbuffers::Offset<flatbuffers::String> offsetComboSectionName = 0;
		const auto sectionNameBuffLen = comboSectionName.Len() + 1;
		if (sectionNameBuffLen > 1)
		{
			auto cComboSectionName = new char[sectionNameBuffLen];
			TPUtil::GetInstance().WCharToMultiByte(cComboSectionName, sectionNameBuffLen, *comboSectionName);
			offsetComboSectionName = _fbb.CreateString((cComboSectionName));
			delete[] cComboSectionName;
		}		

		TB_InputActionBuilder builder(_fbb);
		builder.add_Location(&stLocation);
		builder.add_Rotation(&stRotation);
		builder.add_ComboSectionName(offsetComboSectionName);	
		builder.add_IsCombatPosture(isCombatPosture);
		return builder.Finish();
	}
};
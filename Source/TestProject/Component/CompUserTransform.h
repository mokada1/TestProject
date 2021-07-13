#pragma once

#include "TPComponent.h"
#include "CompUserTransform.generated.h"

UCLASS(BlueprintType)
class UCompUserTransform : public UTPComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FVector BP_GetLocation() const;

	UFUNCTION(BlueprintCallable)
	FVector BP_GetRotation() const;

	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserTransform> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UCompUserTransform* Create(const TB_CompUserTransform& tb);
	~UCompUserTransform();

	Vector3 GetLocation() const;
	Vector3 GetRotation() const;

private:
	void Init();
	void Init(const Vector3 _location, const Vector3 _rotation);

	bool isValid;
	Vector3 location;
	Vector3 rotation;
};
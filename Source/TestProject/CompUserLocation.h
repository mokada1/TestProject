#pragma once

#include "TPComponent.h"
#include "CompUserLocation.generated.h"

UCLASS(BlueprintType)
class UCompUserLocation : public UTPComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FVector BP_GetLocation() const;

	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserLocation> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UCompUserLocation* Create(const float _x, const float _y, const float _z);
	~UCompUserLocation();
	Vector3 GetLocation() const;

private:
	void Init();
	void Init(const float _x, const float _y, const float _z);
	void Init(const double _x, const double _y, const double _z);
	void Init(const Vector3 _location);

	bool isValid;
	Vector3 location;
};
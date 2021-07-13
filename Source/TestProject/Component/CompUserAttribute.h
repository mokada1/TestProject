#pragma once

#include "TPComponent.h"
#include "CompUserAttribute.generated.h"

UCLASS(BlueprintType)
class UCompUserAttribute : public UTPComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float BP_GetHp() const;

	UFUNCTION(BlueprintCallable)
	float BP_GetStr() const;

	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserAttribute> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UCompUserAttribute* Create(const TB_CompUserAttribute& tb);
	~UCompUserAttribute();

	float GetHp() const;
	float GetStr() const;

private:
	void Init();
	void Init(const float _hp, const float _str);

	bool isValid;
	float hp;
	float str;
};
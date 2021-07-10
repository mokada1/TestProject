#pragma once

#include "TPComponent.h"
#include "CompUserCondition.generated.h"

UCLASS(BlueprintType)
class UCompUserCondition : public UTPComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool BP_GetIsCombatPosture() const;

	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserCondition> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UCompUserCondition* Create(const bool _isCombatPosture);
	~UCompUserCondition();

	bool GetIsCombatPosture() const;

private:
	void Init();
	void Init(const bool _isCombatPosture);

	bool isValid;
	bool isCombatPosture;
};
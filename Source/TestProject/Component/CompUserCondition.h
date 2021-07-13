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

	UFUNCTION(BlueprintCallable)
	bool BP_GetIsDied() const;

	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserCondition> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UCompUserCondition* Create(const TB_CompUserCondition& tb);
	~UCompUserCondition();

	bool GetIsCombatPosture() const;
	bool GetIsDied() const;

private:
	void Init();
	void Init(const bool _isCombatPosture, const bool _isDied);

	bool isValid;
	bool isCombatPosture;
	bool isDied;
};
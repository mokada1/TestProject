#pragma once

#include "TP_generated.h"
#include "TPUtil.h"
#include "UObject/Object.h"
#include "TPComponent.generated.h"

UCLASS(BlueprintType)
class UTPComponent : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsValid() const
	{
		return false;
	}
	virtual ~UTPComponent()
	{
	}
};
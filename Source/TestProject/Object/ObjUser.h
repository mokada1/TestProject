#pragma once

#include "TPObject.h"
#include "../Component/CompUserTransform.h"
#include "../Component/CompUserCondition.h"
#include "ObjUser.generated.h"

UCLASS(BlueprintType)
class TESTPROJECT_API UObjUser : public UTPObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString BP_GetUserId() const;

	UFUNCTION(BlueprintCallable)
	UCompUserTransform* BP_GetCompTransform() const;

	UFUNCTION(BlueprintCallable)
	UCompUserCondition* BP_GetCompCondition() const;

	virtual wchar_t* GetObjectId() const override;
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_ObjUser> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UObjUser* Create(wchar_t* const _userId);	
	~UObjUser();

	wchar_t* GetUserId() const;

	UCompUserTransform* GetCompTransform() const;
	UCompUserCondition* GetCompCondition() const;

	void SetCompTransform(UCompUserTransform* const _compTransform);
	void SetCompCondition(UCompUserCondition* const _compCondition);

private:
	void Init();
	void Init(wchar_t* const _userId);

	wchar_t* userId;
	UCompUserTransform* compTransform;
	UCompUserCondition* compCondition;
};
#pragma once

#include "TPObject.h"
#include "../Component/CompUserTransform.h"
#include "../Component/CompUserCondition.h"
#include "../Component/CompUserAttribute.h"
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

	UFUNCTION(BlueprintCallable)
	UCompUserAttribute* BP_GetCompAttribute() const;

	virtual wchar_t* GetObjectId() const override;
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_ObjUser> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UObjUser* Create(const TB_ObjUser& tb);
	~UObjUser();

	wchar_t* GetUserId() const;

	UCompUserTransform* GetCompTransform() const;
	UCompUserCondition* GetCompCondition() const;
	UCompUserAttribute* GetCompAttribute() const;

	void SetCompTransform(UCompUserTransform* const _compTransform);
	void SetCompCondition(UCompUserCondition* const _compCondition);
	void SetCompAttribute(UCompUserAttribute* const _compAttribute);

private:
	void Init();
	void Init(wchar_t* const _userId);

	wchar_t* userId;
	UCompUserTransform* compTransform;
	UCompUserCondition* compCondition;
	UCompUserAttribute* compAttribute;
};
#pragma once

#include "TPObject.h"
#include "../Component/CompUserLocation.h"
#include "ObjUser.generated.h"

UCLASS(BlueprintType)
class TESTPROJECT_API UObjUser : public UTPObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString BP_GetUserId() const;

	UFUNCTION(BlueprintCallable)
	UCompUserLocation* BP_GetCompUserLocation() const;

	virtual wchar_t* GetObjectId() const override;
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_ObjUser> SerializeFB(flatbuffers::FlatBufferBuilder& _fbb) const;

	static UObjUser* Create(wchar_t* const _userId, wchar_t* const _password);	
	~UObjUser();

	wchar_t* GetUserId() const;
	wchar_t* GetPassword() const;

	UCompUserLocation* GetCompUserLocation() const;
	void SetCompUserLocation(UCompUserLocation* const _userLocation);

private:
	void Init();
	void Init(wchar_t* const _userId, wchar_t* const _password);

	wchar_t* userId;
	wchar_t* password;
	UCompUserLocation* userLocation;
};
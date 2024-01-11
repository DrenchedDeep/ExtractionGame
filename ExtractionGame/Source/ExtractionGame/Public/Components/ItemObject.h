

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

enum ERarityType : uint8;
enum EItemTypes : uint8;
enum class EGemType : uint8;

/**
 * 
 */
UCLASS( Blueprintable, BlueprintType )
class EXTRACTIONGAME_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetDimensions() const { return bRotated ? FVector2D(Dimensions.Y, Dimensions.X) : Dimensions;}
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMaterialInterface* GetIcon() const { return bRotated ? IconRotated : Icon; }

	UFUNCTION(BlueprintCallable)
	void Rotate();
	
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	FVector2D Dimensions;
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	UMaterialInterface* Icon;
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	UMaterialInterface* IconRotated;
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	FString ItemName;
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	TEnumAsByte<ERarityType> Rarity;
	UPROPERTY(BlueprintReadWrite, Category = "Default", meta = (ExposeOnSpawn="true"))
	FString Description;
	UPROPERTY(BlueprintReadWrite, Category = "Gems", meta = (ExposeOnSpawn="true"))
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(BlueprintReadWrite, Category = "Gems", meta = (ExposeOnSpawn="true"))
	float DefaultPolish;
	
	//make sure our item has a valid uworld
	virtual UWorld* GetWorld() const override
	{
		if(const UObject* Outer = GetOuter())
		{
			return Outer->GetWorld();
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwningActor() const { return GetTypedOuter<AActor>();}

protected:
//	virtual bool IsSupportedForNetworking() const override { return true; }
private:
	UPROPERTY()
	bool bRotated;
	
};

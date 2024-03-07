

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemObject.generated.h"

enum ERarityType : uint8;
enum EItemTypes : uint8;
enum  EGemType : uint8;

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

	void Init(FVector2D Size,
		UMaterialInterface* InIcon,
		UMaterialInterface* InIconRotated,
		FString InName,
		EItemTypes InType,
		ERarityType InRarity,
		FString Desc,
		EGemType InGemType,
		float InPolish,
		FName InRowName)
	{
		Dimensions = Size;
		this->Icon = InIcon;
		this->IconRotated = InIconRotated;
		ItemName = InName;
		ItemType = InType;
		this->Rarity = InRarity;
		Description = Desc;
		this->GemType = InGemType;
		DefaultPolish = InPolish;
		RowName = InRowName;
	}
	UPROPERTY(BlueprintReadOnly, Replicated)
	FName RowName;
	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector2D Dimensions;
	UPROPERTY(BlueprintReadOnly, Replicated)
	UMaterialInterface* Icon;
	UPROPERTY(BlueprintReadOnly, Replicated)
	UMaterialInterface* IconRotated;
	UPROPERTY(BlueprintReadOnly, Replicated)
	FString ItemName;
	UPROPERTY(BlueprintReadOnly, Replicated)
	TEnumAsByte<EItemTypes> ItemType;
	UPROPERTY(BlueprintReadOnly, Replicated)
	TEnumAsByte<ERarityType> Rarity;
	UPROPERTY(BlueprintReadOnly, Replicated)
	FString Description;
	UPROPERTY(BlueprintReadOnly, Replicated)
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(BlueprintReadOnly, Replicated)
	float DefaultPolish;

	UFUNCTION(BlueprintCallable)
	void SetRarity(ERarityType InRarity) { Rarity = InRarity; }
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginDestroy() override;
private:
	UPROPERTY()
	bool bRotated;
	
};

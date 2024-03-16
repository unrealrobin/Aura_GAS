// Copyright Unreal Robin

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();

	/*Handles our custom calculation*/
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	/* The Attributes we want to capture and use within our Custom Calc.*/
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};

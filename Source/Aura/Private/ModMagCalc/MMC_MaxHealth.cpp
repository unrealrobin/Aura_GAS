// Copyright Unreal Robin


#include "ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	/* Specifying the attribute we want to capture
	 * We are using the Attribute Accessors from our AttributeSet
	 */
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	/* Are we capturing from source or target? */
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	/* When effect is created or when effect is applied
	 * False means we are capturing when effect is created
	 */
	VigorDef.bSnapshot = false;

	/* An Array of Attributes to Capture */
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	
	// Passing in the Source and Target Tags to the Evaluation Parameters
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	// ^ at this point, the Vigor variable has the value of the Vigor attribute of the target

	Vigor = FMath::Max<float>(Vigor, 0.f);
	// ^ at this point, the Vigor variable is guaranteed to be at least 0

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + (Vigor * 2.5f) + (PlayerLevel * 10.f);
}

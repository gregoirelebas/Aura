// Copyright Grégoire Lebas


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.0f);
	InitMaxHealth(100.0f);
	InitMana(50.0f);
	InitMaxMana(50.0f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& EffectProperties) const
{
	const FGameplayEffectContextHandle EffectContext = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceAbilityComponent = EffectContext.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(SourceAbilityComponent))
	{
		if (SourceAbilityComponent->AbilityActorInfo.IsValid() &&
			SourceAbilityComponent->AbilityActorInfo->AvatarActor.IsValid())
		{
			AActor* SourceAvatarActor = SourceAbilityComponent->AbilityActorInfo->AvatarActor.Get();
			AController* SourceController = SourceAbilityComponent->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceAvatarActor != nullptr)
			{
				if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			EffectProperties.EffectContextHandle = EffectContext;
			EffectProperties.SourceAbilityComponent = SourceAbilityComponent;
			EffectProperties.SourceAvatarActor = SourceAvatarActor;
			EffectProperties.SourceController = SourceController;
		}

		if (Data.Target.AbilityActorInfo.IsValid() &&
			Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

			EffectProperties.TargetAvatarActor = TargetAvatarActor;
			EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
			EffectProperties.TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
			EffectProperties.TargetAbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
				TargetAvatarActor);
		}
	}
}

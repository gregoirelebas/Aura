// Copyright Grégoire Lebas


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraEnemy::HighlightActor()
{
	EnableCustomRenderDepth(GetMesh());
	EnableCustomRenderDepth(Weapon);
}

void AAuraEnemy::UnHighlightActor()
{
	DisableCustomRenderDepth(GetMesh());
	DisableCustomRenderDepth(Weapon);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AAuraEnemy::EnableCustomRenderDepth(UPrimitiveComponent* Component)
{
	Component->SetRenderCustomDepth(true);
	Component->SetCustomDepthStencilValue(CUSTOM_RENDER_DEPTH);
}

void AAuraEnemy::DisableCustomRenderDepth(UPrimitiveComponent* Component)
{
	Component->SetRenderCustomDepth(false);
	Component->SetCustomDepthStencilValue(0);
}

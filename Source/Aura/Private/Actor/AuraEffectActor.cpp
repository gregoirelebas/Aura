// Copyright Grégoire Lebas


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASCInterface)
	{
		UAbilitySystemComponent* AbilitySystemComponent = ASCInterface->GetAbilitySystemComponent();
		check(AbilitySystemComponent);

		const UAuraAttributeSet* AuraAttributes = CastChecked<UAuraAttributeSet>(
			AbilitySystemComponent->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		//TODO: Change this to apply GameplayEffect. For now we are using const_cast as a hack...
		UAuraAttributeSet* MutableAuraAttributes = const_cast<UAuraAttributeSet*>(AuraAttributes);
		MutableAuraAttributes->SetHealth(AuraAttributes->GetHealth() + 25.0f);
		MutableAuraAttributes->SetMana(AuraAttributes->GetMana() - 25.0f);
		//MutableAuraAttributes->SetMana(AuraAttributes->GetMana() - 25.0f);

		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlap);
}

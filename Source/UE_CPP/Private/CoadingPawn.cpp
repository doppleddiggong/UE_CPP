#include "CoadingPawn.h"
#include "ULog.h"

#include "CoffeeLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/InputComponent.h"

#define MAPPING_PATH	TEXT("/Game/CustomContents/Inputs/IMC_PlayerInput.IMC_PlayerInput")

// Sets default values
ACoadingPawn::ACoadingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingFinder(MAPPING_PATH);
	if (MappingFinder.Succeeded())
		MappingContext = MappingFinder.Object;
}

// Called when the game starts or when spawned
void ACoadingPawn::BeginPlay()
{
	Super::BeginPlay();

	this->SetupInputMapping();
	
	{
		// --- ULOG 기능 테스트 ---
		ULOG( Warning, "Hello World");
		ULOG( Warning, "Number : %d", Number);
		// 결과: "9,999" (en-US 기준)
		FText LocalizedNumber = FText::AsNumber(Number);
		ULOG( Warning, "#,##0 : %s", *LocalizedNumber.ToString());
		ULOG( Warning, "pi : %f", Pi);
		ULOG( Warning, "longValue : %lld", LongValue);
		ULOG( Warning, "doubleValue : %f", DoubleValue);
		ULOG( Warning, "isGood : %d", IsGood);
		ULOG( Warning, "myName : %s", *MyName);
		ULOG( Warning, "myName : %s", TEXT("배주백"));
	}

	{
		// --- CoffeeLibrary 기능 테스트 ---
		ULOG(Warning, "--- Testing CoffeeLibrary::CommonUtil::InBounds ---");

		// MyArrayCount는 'InBounds' 함수를 테스트하기 위한 가상의 배열 크기입니다.
		const int32 MyArrayCount = 5;
		const int32 TestIndex1 = 3;  // 유효한 인덱스
		const int32 TestIndex2 = 5;  // 유효하지 않은 인덱스

		const bool bIsIndex1InBounds = UCoffeeCommonUtil::InBounds(TestIndex1, MyArrayCount);
		ULOG(Warning, "Is index %d in bounds [0..%d)? -> %s",
			TestIndex1, MyArrayCount, bIsIndex1InBounds ? TEXT("True") : TEXT("False"));

		const bool bIsIndex2InBounds = UCoffeeCommonUtil::InBounds(TestIndex2, MyArrayCount);
		ULOG(Warning, "Is index %d in bounds [0..%d)? -> %s",
			TestIndex2, MyArrayCount, bIsIndex2InBounds ? TEXT("True") : TEXT("False"));
	}
}

void ACoadingPawn::SetupInputMapping() const
{
	if (MappingContext == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MappingContext is null."));
		return;
	}

	if (const auto PlayerCtrl = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCtrl->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
			return;
		}
	}
}

// Called every frame
void ACoadingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACoadingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int32 ACoadingPawn::NewMyAdd(int32 a, int32 b)
{
	return a + b;
}
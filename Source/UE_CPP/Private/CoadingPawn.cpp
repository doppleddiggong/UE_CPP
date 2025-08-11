
#include "CoadingPawn.h"
#include "ULog.h"

#include "CoffeeLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "Bullet.h"


#define MAPPING_PATH	TEXT("/Game/CustomContents/Inputs/IMC_PlayerInput.IMC_PlayerInput")

ACoadingPawn::ACoadingPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingFinder(MAPPING_PATH);
	if (MappingFinder.Succeeded())
		MappingContext = MappingFinder.Object;
}

void ACoadingPawn::BeginPlay()
{
	Super::BeginPlay();

	//
	// // 컬러값에서 r,g,b 뽑아내기
	// int32 color = 0xffaaccee;
	// int32 r = color >> 24; // 0xff
	// int32 g = (color >> 16) & r; // 0xffaa
	// int32 b = (color >> 8) & r;
	// int32 a = (color >> 8);
	//
	// int32 color2 = 0;
	// color2 = (r << 24) | (g << 16 ) | (b << 8 ) | a ;

	
	// ULOG( Warning, "%d, %d, %d",  r, g, b);
		
	// FDateTime DateTime(1986,11,8);
	// ULOG( Warning, "%d",  DateTime.GetYear());
	// ULOG( Warning, "%d",  DateTime.GetMonth());
	// ULOG( Warning, "%d",  DateTime.GetDay());

	
	ULOG( Warning, "GetNowTimestamp :  %lld", UCoffeeCommonUtil::GetNowTimestamp() );


	// int32 ymd = 19'861'108;
	// int32 year = ymd/10'000;
	// int32 month =(ymd -year*10000)/1'00;
	// int32 day = (ymd -year*10000 - month*100);
	// ULOG( Warning, "%d",  ymd);
	// ULOG( Warning, "%d",  year);
	// ULOG( Warning, "%d",  month);
	// ULOG( Warning, "%d",  day);
	

	this->SetupInputMapping();
/*
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
		constexpr int32 MyArrayCount = 5;
		constexpr int32 TestIndex1 = 3;  // 유효한 인덱스
		constexpr int32 TestIndex2 = 5;  // 유효하지 않은 인덱스

		const bool bIsIndex1InBounds = UCoffeeCommonUtil::InBounds(TestIndex1, MyArrayCount);
		ULOG(Warning, "Is index %d in bounds [0..%d)? -> %s",
			TestIndex1, MyArrayCount, bIsIndex1InBounds ? TEXT("True") : TEXT("False"));

		const bool bIsIndex2InBounds = UCoffeeCommonUtil::InBounds(TestIndex2, MyArrayCount);
		ULOG(Warning, "Is index %d in bounds [0..%d)? -> %s",
			TestIndex2, MyArrayCount, bIsIndex2InBounds ? TEXT("True") : TEXT("False"));
	}
	*/
}

void ACoadingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->UpdateMove(DeltaTime);
}

void ACoadingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if ( UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(HorizontalAction, ETriggerEvent::Triggered, this, &ACoadingPawn::OnHorizontalAction );
		EnhancedInput->BindAction(HorizontalAction, ETriggerEvent::Completed , this, &ACoadingPawn::OnHorizontalAction );

		EnhancedInput->BindAction(VerticalAction, ETriggerEvent::Triggered, this, &ACoadingPawn::OnVerticalAction );
		EnhancedInput->BindAction(VerticalAction, ETriggerEvent::Completed, this, &ACoadingPawn::OnVerticalAction );

		EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ACoadingPawn::OnFireAction);
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
		}
	}
}

void ACoadingPawn::UpdateMove(float DeltaTime)
{
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Horizontal: %.2f, Vertical: %.2f"), HorizontalValue, VerticalValue);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, Msg);
	}

	auto Move = FVector(0.0f, HorizontalValue, VerticalValue);
	if ( Move.IsNearlyZero() == false )
		Move = Move.GetSafeNormal() * MoveSpd * DeltaTime;
	
	this->SetActorLocation( GetActorLocation() + Move );
}

void ACoadingPawn::OnHorizontalAction(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();

	this->HorizontalValue = AxisValue;
	ULOG( Warning, "OnHorizontalAction : %f", HorizontalValue);
	
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorRightVector(), AxisValue);
	}
}

void ACoadingPawn::OnVerticalAction(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();

	this->VerticalValue = AxisValue;
	ULOG( Warning, "OnVerticalAction : %f", VerticalValue);

	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorUpVector(), AxisValue);
	}
}

void ACoadingPawn::OnFireAction(const FInputActionValue& Value)
{
	if ( BulletClass == nullptr )
		return;
	
	UWorld* World = GetWorld();
	auto SpawnLocation = FirePos->GetComponentLocation();
	auto SpawnRotation = FirePos->GetComponentRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	
	ABullet* Bullet = World->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);

	UGameplayStatics::PlaySound2D(World, FireSound);
}
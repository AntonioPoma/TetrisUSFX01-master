// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Observer.h"

#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

// Sets default values

ABoard::ABoard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();

    AObserver* Observer1 = GetWorld()-> SpawnActor<AObserver>();
    AddObserver(Observer1);

    NextPiece = GetWorld()->SpawnActor<APiece>(FVector(0.0f, 100.0f, 150.0f), FRotator(0.0f, 0.0f, 0.0f));
    SubsequentPiece = GetWorld()->SpawnActor<APiece>(FVector(0.0f, 100.0f, 100.0f), FRotator(0.0f, 0.0f, 0.0f));

	
    for (TActorIterator<APiece> it(GetWorld()); it; ++it)
    {
        if (it->GetFName() == TEXT("DissmissPieces"))
        {
            it->Dismiss();
            it->Destroy();
        }
    }
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bGameOver)
    {
        return;
    }

    switch (Status)
    {
    case PS_NOT_INITED:
        NewPiece();
        CoolLeft = CoolDown;
        Status = PS_MOVING;
        break;
    case PS_MOVING:
        CoolLeft -= DeltaTime;
        if (CoolLeft <= 0.0f)
        {
            MoveDown();
        }
        break;
    case PS_GOT_BOTTOM:
        CoolLeft -= DeltaTime;
        if (CoolLeft <= 0.0f)
        {
            if (CurrentPiece)
            {
                CurrentPiece->Dismiss();
                CurrentPiece->Destroy();
            }
            CurrentPiece = nullptr;
            NewPiece();
            CoolLeft = CoolDown;
            Status = PS_MOVING;
        }
        break;
    default:
        break;
    }
}

// Called to bind functionality to input
void ABoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &ABoard::Rotate);
    PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABoard::MoveLeft);
    PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ABoard::MoveRight);
    PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ABoard::MoveDown);
    PlayerInputComponent->BindAction("MoveDownToEnd", IE_Pressed, this, &ABoard::MoveDownToEnd);
    PlayerInputComponent->BindAction("NewPiece", IE_Pressed, this, &ABoard::NewPiece);
    PlayerInputComponent->BindAction("reset", IE_Pressed, this, &ABoard::resetBoard);
}

void ABoard::resetBoard()
{
    UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetFName(), false);
}

void ABoard::Rotate()
{
    if (CurrentPiece && Status != PS_GOT_BOTTOM)
    {
        CurrentPiece->TestRotate();
    }
}

void ABoard::MoveLeft()
{
    if (CurrentPiece)
    {
        CurrentPiece->MoveLeft();
        if (Status == PS_GOT_BOTTOM)
        {
            MoveDownToEnd();
        }
    }
}

void ABoard::MoveRight()
{
    if (CurrentPiece)
    {
        CurrentPiece->MoveRight();
        if (Status == PS_GOT_BOTTOM)
        {
            MoveDownToEnd();
        }
    }
}

void ABoard::MoveDown()
{
    if (CurrentPiece)
    {
        if (!CurrentPiece->MoveDown())
        {
            Status = PS_GOT_BOTTOM;
        }
        CoolLeft = CoolDown;
    }
}

bool ABoard::CheckGameOver() {
    return CurrentPiece->CheckWillCollision([](FVector OldVector) {
        return OldVector;
        }
    );

    if (!CurrentPiece) {
        UE_LOG(LogTemp, Warning, TEXT("NoPieces"));
        return true;
    }
}

void ABoard::NotifyLineCompleted()
{
    for (IInterfaz_Observer* Observer : Observers) {
        Observer->OnLineCompleted();
    }
}

void ABoard::NotifyGameOver()
{
    for (IInterfaz_Observer* Observer : Observers) {
        Observer->OnGameOver();
    }
}

void ABoard::ScoreObserver()
{
    for (IInterfaz_Observer* Observer : Observers) {
        Observer->Score();
    }
}

void ABoard::BlocksDestroyedObserver()
{
    for (IInterfaz_Observer* Observer : Observers) {
        Observer->BlocksDestroyed();
    }
}

void ABoard::NumPiecesChangedObserver()
{
    for (IInterfaz_Observer* Observer : Observers) {
        Observer->NumPiecesChanged();
    }
}

void ABoard::NumLinesChangedObserver()
{
    for (IInterfaz_Observer* Observer : Observers) {
        Observer->OnLineCompleted();
    }
}



void ABoard::NewPiece()
{
    CheckLine();
    if (CurrentPiece)
    {
        CurrentPiece->Dismiss();
        CurrentPiece->Destroy();
    }

    BlocksDestroyedObserver();
    NumPiecesChangedObserver();
    ScoreObserver();

    CurrentPiece = NextPiece;
    CurrentPiece->SetActorLocation(FVector(0.0f, 5.0f, 195.0f));
    CurrentPiece->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));

    NextPiece = SubsequentPiece;
    NextPiece->SetActorLocation(FVector(0.0f, 100.0f, 150.0f));
    NextPiece->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));

    SubsequentPiece = GetWorld()->SpawnActor<APiece>(FVector(0.0f, 100.0f, 100.0f), FRotator(0.0f, 0.0f, 0.0f));


    bGameOver = CheckGameOver();
    if (bGameOver)
    {
        NotifyGameOver();
        UE_LOG(LogTemp, Warning, TEXT("Game Over!!!!!!!!"));
    }
}

void ABoard::CheckLine()
{
    auto MoveDownFromLine = [this](int z) {
        FVector Location(0.0f, 0.0f, 5.0 * z + 100.0);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<struct FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        FVector Extent(4.5f, 49.5f, 95.0 + 4.5 - 5.0 * z);
        CollisionShape.SetBox(Extent);
        //DrawDebugBox(GetWorld(), Location, Extent, FColor::Purple, false, 1, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        if (GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam))
        {
            for (auto&& Result : OutOverlaps)
            {
                FVector NewLocation = Result.GetActor()->GetActorLocation();
                NewLocation.Z -= 10.0f;
                Result.GetActor()->SetActorLocation(NewLocation);
            }
        }
    };

    int z = 0;
    int LinesEliminatedCount = 0;
    while (z < 20) {
        FVector Location(0.0f, 0.0f, 10.0f * z + 5.0f);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        TArray<FOverlapResult> OutOverlaps;
        FCollisionShape CollisionShape;
        CollisionShape.SetBox(FVector(4.0f, 49.0f, 4.0f));
        //DrawDebugBox (GetWorld (), Location, FVector (4.5f, 49.5f, 4.5f), FColor::Purple, false, 0, 0, 1);
        FCollisionQueryParams Params;
        FCollisionResponseParams ResponseParam;
        bool b = GetWorld()->OverlapMultiByChannel(OutOverlaps,
            Location, Rotation.Quaternion(), ECollisionChannel::ECC_WorldDynamic,
            CollisionShape, Params, ResponseParam);
        if (!b || OutOverlaps.Num() < 10) {
            ++z;
            continue;
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Find FULL LINE at z=%d"), z);
            GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Find FULL LINE"));
            ScoreObserver(); ScoreObserver();
            NumLinesChangedObserver();
            for (auto&& Result : OutOverlaps) {
                Result.GetActor()->Destroy();
            }

            MoveDownFromLine(z);
            NotifyLineCompleted();

            LinesEliminatedCount++;
            if (LinesEliminatedCount == 2 && !bModeTuned) {
                bModeTuned = true;
                LinesEliminatedCount = 0;
                GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Tuneado Mode Activated!"));
            }
            else if (bModeTuned && LinesEliminatedCount >= 2) {
                LinesEliminatedCount = 0;
                z -= 1; // Recheck the same line again to potentially remove the next consecutive line
                GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Consecutive Line Eliminated!"));

                NumLinesChangedObserver(); NumLinesChangedObserver();

                ScoreObserver();
                ScoreObserver();
                ScoreObserver();
            }
        }
    }
}

void ABoard::MoveDownToEnd() {
    if (!CurrentPiece) {
        return;
    }

    while (CurrentPiece->MoveDown()) {

    }

    UE_LOG(LogTemp, Warning, TEXT("se fue al final"));

    switch (Status) {
    case PS_MOVING:
        Status = PS_GOT_BOTTOM;
        CoolLeft = CoolDown;
        break;
    case PS_GOT_BOTTOM:
        if (bModeTuned) {
            // Eliminar 2 líneas consecutivas si están completas
            CheckLine();
            CheckLine();
        }
        else {
            //CheckLine ();
        }

        CoolLeft = CoolDown;
        Status = PS_MOVING;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Wrong status for MoveDownToEnd"));
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Wrong status for MoveDownToEnd"));
        break;
    }
}

void ABoard::AddObserver(IInterfaz_Observer* Observer)
{
    Observers.Add(Observer);
}

void ABoard::RemoveObserver(IInterfaz_Observer* Observer)
{
    Observers.Remove(Observer);
}

void ABoard::TextNextPiece()
{
    FVector TextLocation(0.0f, 80.0f, 170.0f);
    FColor TextColor(128, 255, 212); // Color verde agua

    // Crear un actor de texto en el mundo
    ATextRenderActor *TextActor = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextActor) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextActor->GetTextRender();
        if (TextComponent) {
            // Configurar el texto y el color
            TextComponent->SetText(FText::FromString("Next Piece"));
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(7.0f);
            TextComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }
}

void ABoard::TextSubsequentPiece()
{
    FVector TextLocation(0.f, 70.0f, 120.f);
    FColor TextColor(128, 255, 212); // Color verde agua

    // Crear un actor de texto en el mundo
    ATextRenderActor* TextActor = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextActor) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextActor->GetTextRender();
        if (TextComponent) {
            // Configurar el texto y el color
            TextComponent->SetText(FText::FromString("Subsequent Piece"));
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(7.0f);
            TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }
}


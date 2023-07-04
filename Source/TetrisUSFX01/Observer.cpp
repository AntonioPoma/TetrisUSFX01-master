// Fill out your copyright notice in the Description page of Project Settings.
#include "Observer.h"

#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

AObserver::AObserver()
{

}

void AObserver::OnLineCompleted() {
    TextNumLines();
    numLines++;
}

void AObserver::OnGameOver() {
    TextGameOver();
    score = 0;
}

void AObserver::BlocksDestroyed() {
    TextNumBlocks();
    numBLocks += 4;
}

void AObserver::NumPiecesChanged() {
    TextNumPieces();
    numPieces++;
}

void AObserver::Score() {
    TextScore();
    score += 5;
}

void AObserver::TextScore() {
    // Destruir el actor de texto anterior
    if (TextActor) {
        TextActor->Destroy();
        TextActor = nullptr;
    }

    FVector TextLocation(0.f, -100.0f, 120.f);
    FColor TextColor(128, 255, 212); // Color verde agua

    FString ScoreText = FString::Printf(TEXT("Score: %d"), score);

    // Crear un actor de texto en el mundo
    TextActor = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextActor) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextActor->GetTextRender();
        if (TextComponent) {
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(10.0f);
            TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

            // Configurar el texto con el valor del score
            TextComponent->SetText(FText::FromString(ScoreText));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }
}


void AObserver::TextNumPieces() {
    // Destruir el actor de texto anterior
    if (TextPieces) {
        TextPieces->Destroy();
        TextPieces = nullptr;
    }

    FVector TextLocation(0.f, -120.0f, 105.f);
    FColor TextColor(128, 255, 212); // Color verde agua

    FString ScoreText = FString::Printf(TEXT("Num Pieces: %d"), numPieces);

    // Crear un actor de texto en el mundo
    TextPieces = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextPieces) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextPieces->GetTextRender();
        if (TextComponent) {
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(10.0f);
            TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

            // Configurar el texto con el valor del score
            TextComponent->SetText(FText::FromString(ScoreText));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }

}

void AObserver::TextNumBlocks() {
    // Destruir el actor de texto anterior
    if (TextBlocks) {
        TextBlocks->Destroy();
        TextBlocks = nullptr;
    }

    FVector TextLocation(0.f, -120.0f, 90.f);
    FColor TextColor(128, 255, 212); // Color verde agua

    FString ScoreText = FString::Printf(TEXT("Num BLocks: %d"), numBLocks);

    // Crear un actor de texto en el mundo
    TextBlocks = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextBlocks) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextBlocks->GetTextRender();
        if (TextComponent) {
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(10.0f);
            TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

            // Configurar el texto con el valor del score
            TextComponent->SetText(FText::FromString(ScoreText));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }
}

void AObserver::TextNumLines() {
    // Destruir el actor de texto anterior
    if (TextLines) {
        TextLines->Destroy();
        TextLines = nullptr;
    }

    FVector TextLocation(0.f, -130.0f, 70);
    FColor TextColor(128, 255, 212); // Color verde agua

    FString ScoreText = FString::Printf(TEXT("Lines Complets: %d"), numLines);

    // Crear un actor de texto en el mundo
    TextLines = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextLines) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextLines->GetTextRender();
        if (TextComponent) {
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(10.0f);
            TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

            // Configurar el texto con el valor del score
            TextComponent->SetText(FText::FromString(ScoreText));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }
}

void AObserver::TextGameOver() {
    FVector TextLocation(30.0f, 0.0f, 100.0f);
    FColor TextColor(128, 255, 212); // Color verde agua

    FString GameOverText = FString::Printf(TEXT("GAME OVER"));

    // Crear un actor de texto en el mundo
    ATextRenderActor* TextGO = GetWorld()->SpawnActor<ATextRenderActor>(TextLocation, FRotator::ZeroRotator);
    if (TextGO) {
        // Obtener el componente de texto renderizado
        UTextRenderComponent* TextComponent = TextGO->GetTextRender();
        if (TextComponent) {
            TextComponent->SetTextRenderColor(TextColor);

            // Otras configuraciones opcionales
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
            TextComponent->SetWorldSize(50.0f);
            TextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

            // Configurar el texto con el valor del score
            TextComponent->SetText(FText::FromString(GameOverText));

            // Asegurarse de que el texto sea visible en el juego
            TextComponent->SetHiddenInGame(false);
        }
    }
}

void AObserver::ResetObserver() {
    numPieces = 0;
    score = 0;
    numBLocks = 0;
    numLines = 0;
}



// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Piece.h"
#include "Interfaz_Observer.h"
#include "Board.generated.h"

UCLASS()
class TETRISUSFX01_API ABoard : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
	APiece* CurrentPiece;
	APiece* NextPiece;
	APiece* SubsequentPiece;
	
	void resetBoard();
	void Rotate(); 
	void MoveLeft(); 
	void MoveRight();
	void MoveDown(); 
	void NewPiece();
	void CheckLine();
	void MoveDownToEnd();

	void AddObserver(IInterfaz_Observer* Observer);
	void RemoveObserver(IInterfaz_Observer* Observer);

	void TextNextPiece();
	void TextSubsequentPiece();

private:
	enum PieceStatus { PS_NOT_INITED, PS_MOVING, PS_GOT_BOTTOM };
	PieceStatus Status = PS_NOT_INITED;

	const float CoolDown = 0.5f;
	float CoolLeft = 0.5f;
	bool bGameOver = false;

	bool CheckGameOver();

	TArray<IInterfaz_Observer*> Observers; // Lista de observadores

	void NotifyLineCompleted();
	void NotifyGameOver();
	void ScoreObserver();
	void BlocksDestroyedObserver();
	void NumPiecesChangedObserver();
	void NumLinesChangedObserver();

private:
	int LinesEliminated = 0;
	bool bModeTuned = false;

};

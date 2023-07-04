// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaz_Observer.h"
#include "Observer.generated.h"

UCLASS()
class TETRISUSFX01_API AObserver : public AActor, public IInterfaz_Observer{

	GENERATED_BODY()
	
public:	
	AObserver();

	virtual void OnLineCompleted() override;
	virtual void OnGameOver() override;
	virtual void BlocksDestroyed() override;
	virtual void NumPiecesChanged() override;
	virtual void Score() override;
	virtual void ResetObserver() override;

	void TextScore();
	void TextNumPieces();
	void TextNumBlocks();
	void TextNumLines();
	void TextGameOver();

	int numPieces = 0;
	int score = 0;
	int numBLocks = 0;
	int numLines = 0;

private:
	class ATextRenderActor* TextActor;
	class ATextRenderActor* TextPieces;
	class ATextRenderActor* TextBlocks;
	class ATextRenderActor* TextLines;
};

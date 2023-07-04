// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interfaz_Observer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterfaz_Observer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TETRISUSFX01_API IInterfaz_Observer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BlocksDestroyed() = 0;
	virtual void NumPiecesChanged() = 0;
	virtual void Score() = 0;
	virtual void OnLineCompleted() = 0;
	virtual void OnGameOver() = 0;
	virtual void ResetObserver() = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/EditableText.h"

#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEDITEST_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FString GetID();
	FString GetPW();

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableText* EditableTextBox_ID;

	UPROPERTY(meta = (BindWidget))
	UEditableText* EditableTextBox_PW;
	
};

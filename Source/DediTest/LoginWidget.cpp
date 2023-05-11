// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

FString ULoginWidget::GetID()
{
	FText InputID = EditableTextBox_ID->GetText();
	//UE_LOG(LogTemp, Error, TEXT("ID"), PortNumber);

	return InputID.ToString();
}

FString ULoginWidget::GetPW()
{
	FText InputPW = EditableTextBox_PW->GetText();

	return InputPW.ToString();
}

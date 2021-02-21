// Fill out your copyright notice in the Description page of Project Settings.


#include "Idle.h"


Idle::Idle(int id)
{
	_id = id;
}

void Idle::Tick() {
	// Literally do nothing. Just wait for a new Target.
	
}

void Idle::OnEnter() {
	// Turn off movement
	// Make sure Behavior Tree logic is off. (Maybe move this responsibility to other states)
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Entered IDLE")));
}

void Idle::OnExit() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Exited IDLE")));
}

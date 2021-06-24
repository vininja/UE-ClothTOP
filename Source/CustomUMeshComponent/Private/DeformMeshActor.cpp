// Fill out your copyright notice in the Description page of Project Settings.


#include "DeformMeshActor.h"
#include "EngineUtils.h"
#include "TouchBlueprintFunctionLibrary.h"
#include "TouchEngineComponent.h"
#include "TouchEngineInfo.h"
#include "TouchEngineDynamicVariableStruct.h"
#include "UTouchEngine.h"

#include "TimerManager.h"

// Sets default values
ADeformMeshActor::ADeformMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DeformMeshComp = CreateDefaultSubobject<UDeformMeshComponent>(TEXT("Deform Mesh Component"));
	Controller = CreateDefaultSubobject<AActor>(TEXT("Controller"));
	//PositionTexture = CreateDefaultSubobject<UTexture2D>(TEXT("Positions Texture"), true);
	//NormalTexture = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("Normals Texture"));
	TouchEngineComponent = CreateDefaultSubobject<UTouchEngineComponentBase>("TouchEngine Component");

	// Defaults 
	TouchEngineComponent->ToxFilePath = "Toxes/clt_cloak.tox";
	TouchEngineComponent->LoadOnBeginPlay = 1;
	//TouchEngineComponent->CookMode = ETouchEngineCookMode::Synchronized;
}

// Called when the game starts or when spawned
void ADeformMeshActor::BeginPlay()
{
	Super::BeginPlay();

	TouchEngineComponent->StartTouchEngine();
	while (TouchEngineComponent->IsLoaded() == false);
	while (TouchEngineComponent->IsRunning() == false);

	DeformMeshComp->CreateMeshSection(0, ClothMesh, TouchEngineComponent,  Controller->GetTransform());
	//DeformMeshComp->SetMeshSectionVisible(0, true);
}

void ADeformMeshActor::RepeatingFunction()
{
	FTouchVar<bool> reset;
	reset.Data = false;
	TouchEngineComponent->EngineInfo->SetBooleanInput("pn/Reset", reset);
}

// Called every frame
void ADeformMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeformMeshComp->UpdateMeshSectionTransform(0, TouchEngineComponent,Controller->GetTransform());
	DeformMeshComp->FinishTransformsUpdate();//We finalize all the deform transforms updates, in our case, just one

}


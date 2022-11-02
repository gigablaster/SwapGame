#include "SwapEditorModule.h"
#include "SwapEditor.h"
#include "Modules/ModuleManager.h"
#include "ComponentVisualizer.h"
#include "ComponentVisualizers.h"
#include "UnrealEd.h"
#include "AI/PatrolRouteComponent.h"
#include "Visualizers/PatrolRouteComponentVisualizer.h"

void FSwapEditorModule::StartupModule()
{
	UE_LOG(LogSwapEditor, Log, TEXT("Module startup"));

	RegisterComponentVisualizers();
}

void FSwapEditorModule::ShutdownModule()
{
	UE_LOG(LogSwapEditor, Log, TEXT("Module shutdown"));

	UnregisterComponentVisualizers();
}

void FSwapEditorModule::RegisterComponentVisualizers()
{
	FComponentVisualizersModule& VisualizersModule = FModuleManager::LoadModuleChecked<FComponentVisualizersModule>(TEXT("ComponentVisualizers"));
	VisualizersModule.RegisterComponentVisualizer(UPatrolRouteComponent::StaticClass()->GetFName(), MakeShareable(new FPatrolRouteComponentVisualizer));
}

void FSwapEditorModule::UnregisterComponentVisualizers()
{
	
}

IMPLEMENT_GAME_MODULE(FSwapEditorModule, SwapEditor);

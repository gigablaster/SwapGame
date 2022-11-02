#pragma once

#include "Modules/ModuleInterface.h"

class FSwapEditorModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
protected:
	void RegisterComponentVisualizers();
	void UnregisterComponentVisualizers();
};
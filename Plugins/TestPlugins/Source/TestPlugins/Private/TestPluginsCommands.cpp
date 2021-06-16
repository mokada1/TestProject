// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestPluginsCommands.h"

#define LOCTEXT_NAMESPACE "FTestPluginsModule"

void FTestPluginsCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "TestPlugins", "Execute TestPlugins action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE

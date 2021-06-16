// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TestPluginsStyle.h"

class FTestPluginsCommands : public TCommands<FTestPluginsCommands>
{
public:

	FTestPluginsCommands()
		: TCommands<FTestPluginsCommands>(TEXT("TestPlugins"), NSLOCTEXT("Contexts", "TestPlugins", "TestPlugins Plugin"), NAME_None, FTestPluginsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};

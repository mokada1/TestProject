// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestPlugins.h"
#include "TestPluginsStyle.h"
#include "TestPluginsCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "EngineUtils.h"

static const FName TestPluginsTabName("TestPlugins");

#define LOCTEXT_NAMESPACE "FTestPluginsModule"

void FTestPluginsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestPluginsStyle::Initialize();
	FTestPluginsStyle::ReloadTextures();

	FTestPluginsCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTestPluginsCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FTestPluginsModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTestPluginsModule::RegisterMenus));
}

void FTestPluginsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTestPluginsStyle::Shutdown();

	FTestPluginsCommands::Unregister();
}

void FTestPluginsModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	auto world = GEditor->GetEditorWorldContext().World();
	auto actorRange = FActorRange(world);
	FString name = "";
	for (const auto& entity : actorRange)
	{
		name.Append(entity->GetName());
		name.Append(",");
	}

	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "{0}"),
							FText::FromString(name)
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FTestPluginsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTestPluginsCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTestPluginsCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestPluginsModule, TestPlugins)
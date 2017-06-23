// Fill out your copyright notice in the Description page of Project Settings.

#include "AirSim.h"
#include "AirSimGameMode.h"
#include "SimHUD/SimHUD.h"



AAirSimGameMode::AAirSimGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    DefaultPawnClass = nullptr;
    HUDClass = ASimHUD::StaticClass();
}

void AAirSimGameMode::StartPlay() 
{
    Super::StartPlay();

    initializeSettings();
}

void AAirSimGameMode::initializeSettings()
{
    //TODO: should this be done somewhere else?
    //load settings file if found
    typedef msr::airlib::Settings Settings;
    try {
        Settings& settings = Settings::loadJSonFile("settings.json");
        auto settings_filename = Settings::singleton().getFileName();
        if (settings.isLoadSuccess()) {
            UAirBlueprintLib::setLogMessagesHidden(! settings.getBool("LogMessagesVisible", true));

            std::string msg = "Loaded settings from " + settings_filename;
            UAirBlueprintLib::LogMessage(FString(msg.c_str()), TEXT(""), LogDebugLevel::Informational);
        }
        else {
            //write some settings in new file otherwise the string "null" is written if all settigs are empty
            settings.setString("see_docs_at", "https://github.com/Microsoft/AirSim/blob/master/docs/settings.md");

            //settings.setBool("RpcEnabled", true);
            //settings.setString("LocalHostIp", "127.0.0.1");
            //Settings rosflight_child;
            //rosflight_child.setInt("RemoteControlID", 0);
            //settings.setChild("RosFlight", rosflight_child);

            settings.saveJSonFile(settings_filename);
            std::string msg = "Settings file " + settings_filename + " is created.";
            UAirBlueprintLib::LogMessage(FString(msg.c_str()), TEXT("See docs at https://git.io/v9mYY"), LogDebugLevel::Informational);
        }
    }
    catch (std::exception ex) {
        UAirBlueprintLib::LogMessage(FString("Error loading settings from ~/Documents/AirSim/settings.json"), TEXT(""), LogDebugLevel::Failure, 30);
        UAirBlueprintLib::LogMessage(FString(ex.what()), TEXT(""), LogDebugLevel::Failure, 30);
    }
}
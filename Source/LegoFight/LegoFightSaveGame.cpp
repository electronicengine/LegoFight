// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoFightSaveGame.h"



ULegoFightSaveGame::ULegoFightSaveGame()
{

}



TSharedPtr<FJsonObject> ULegoFightSaveGame::convertConstructionInfoToJson(const ConstructionInfo &Info)
{

    TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject);
    TArray<TSharedPtr<FJsonValue>> plugged_bricks_onit;


    json->SetNumberField("Plugin_Index", Info.Plugin_Index);
    json->SetStringField("Offset_Rotation", Info.Offset_Rotation.ToString());
    json->SetNumberField("Type", Info.Type_);
    json->SetNumberField("Sub_Type", Info.Sub_Type);

    for(int i=0; i<Info.Plugged_Bricks_OnIt.size(); i++)
    {

        TSharedPtr<FJsonObject> brick_info = MakeShareable(new FJsonObject);
        brick_info = convertConstructionInfoToJson(Info.Plugged_Bricks_OnIt[i]);

        plugged_bricks_onit.Push(MakeShareable(new FJsonValueObject(brick_info)));

    }

    json->SetArrayField("Plugged_Bricks", plugged_bricks_onit);


    return json;

}



FString ULegoFightSaveGame::serializeJsonObject(const TSharedPtr<FJsonObject> &Json)
{
    FString json_string;

    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&json_string);

    FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

    return json_string;
}



ConstructionInfo ULegoFightSaveGame::convertJsonToConstructionInfo(const TSharedPtr<FJsonObject> &Json)
{
    ConstructionInfo main_brick_info;
    TArray<TSharedPtr<FJsonValue>> plugged_bricks_onit = Json->GetArrayField("Plugged_Bricks");

    main_brick_info.Plugin_Index = Json->GetIntegerField("Plugin_Index");
    main_brick_info.Offset_Rotation.InitFromString(Json->GetStringField("Offset_Rotation"));
    main_brick_info.Type_ = Json->GetIntegerField("Type");
    main_brick_info.Sub_Type = Json->GetIntegerField("Sub_Type");



    for(int i=0; i<plugged_bricks_onit.Num(); i++)
    {
        ConstructionInfo brick_info;

        brick_info = convertJsonToConstructionInfo(plugged_bricks_onit[i]->AsObject());

        main_brick_info.Plugged_Bricks_OnIt.push_back(brick_info);
    }


    return main_brick_info;
}



TSharedPtr<FJsonObject> ULegoFightSaveGame::deserializeJsonObject(const FString &JsonString)
{


    TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject);

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    FJsonSerializer::Deserialize(Reader, json);

    return json;
}


#pragma once
namespace Debug
{
	static void PrintString(const FString& Msg, float time=2.f,FColor ColorType=FColor::Green)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, time, ColorType, Msg);
		}
	}
}
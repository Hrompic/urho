#include <android/native_activity.h>
#include <unistd.h>
#include <stdio.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MyApp", __VA_ARGS__))


//#include "1.cpp"
//extern int SDL_main(int argc, char **argv);
extern int android_main(int argc, char **argv);
 void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{	
	FILE *a = fopen("/sdcard/urho/123", "r+");
	if(!a) LOGI("Error: /sdcard/urho/123");
	char *argv[] = {{"libout"},
				{"-p"},
				{"/sdcard/urho/CoreData;/sdcard/urho/Data/"}};
	LOGI("%s, %s, %s",argv[0], argv[1], argv[2]);
	fprintf(a, "%s, %s, %s", argv[0], argv[1], argv[2]);
	fclose(a);
	android_main(3, argv);
}

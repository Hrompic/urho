all:
	g++ 1.cpp -lUrho3D -g -O2 -D__DEBUG__
andr:
	#aarch64-linux-android26-clang++ 1.cpp /home/user/Android/Sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.c -D__ANDROID__ -shared -lUrho3D -lGLESv2 -landroid -o and/lib/arm64-v8a/libout.so
	#aarch64-linux-android26-clang++ 1.cpp -D__ANDROID__ -shared -lUrho3D -lGLESv2 -landroid -o and/lib/arm64-v8a/libout.so
	aarch64-linux-android26-clang++ 1.cpp -D__ANDROID__ -shared -lUrho3D -lGLESv2 -landroid  -o and/lib/arm64-v8a/liba.so

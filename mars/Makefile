all: andr install-andr
#	g++ main.cpp -lUrho3D -g -O2 -D__DEBUG__
andr:
	aarch64-linux-android26-clang++ main.cpp -D__ANDROID__ -shared -lc++_shared -lUrho3D -lGLESv2 -landroid  -o and/lib/arm64-v8a/liba.so
	cd and&&make rlib
install-andr:
	cd and&&make install

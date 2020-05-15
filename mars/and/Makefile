all: init reb sign
init:
	aapt package -f -m -M AndroidManifest.xml -S res/ -J gen -I /home/user/Android/Sdk/platforms/android-26/android.jar 
	kotlinc-jvm ~/source/Urho3D/android/urho3d-lib/src/main/java/com/github/urho3d/UrhoActivity.kt ~/source/Urho3D/Source/ThirdParty/SDL/android-project/app/src/main/java/org/libsdl/app/* -cp /home/user/Android/Sdk/platforms/android-26/android.jar -include-runtime -d obj	
	javac -d obj -cp obj -bootclasspath /home/user/Android/Sdk/platforms/android-26/android.jar gen/org/FUCK/app/R.java ~/source/Urho3D/Source/ThirdParty/SDL/android-project/app/src/main/java/org/libsdl/app/*	
	dx --dex --output=./classes.dex obj
reb:
	aapt package -f -m -F app.apk -A assets/ -M AndroidManifest.xml -S res/ -I /home/user/Android/Sdk/platforms/android-26/android.jar
	aapt add app.apk classes.dex lib/arm64-v8a/*
sign:
	
	zipalign -f 4 app.apk app1.apk
	apksigner sign --ks ~/mykey.keystore app1.apk <~/keypass
rlib:
	aapt r app.apk lib/arm64-v8a/liba.so 
	aapt add app.apk lib/arm64-v8a/liba.so
	$(MAKE) sign
install:
	adb install -r app1.apk 
clean:
	rm -rf app.apk app1.apk	
remake: clean reb sign

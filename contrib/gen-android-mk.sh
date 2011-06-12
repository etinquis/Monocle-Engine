#!/bin/sh

mkdir ../Build ../Build/gen-android-Monocle ../Build/gen-android-Monocle/jni
rm -r ../Build/gen-android-Monocle/jni/*
cp -r ../Core ../Build/gen-android-Monocle/jni/
cp -r ../Libraries ../Build/gen-android-Monocle/jni/

pushd ../Build/gen-android-Monocle/jni
echo "include \$(call all-subdir-makefiles)" 														> Android.mk

echo "APP_PLATFORM			:= android-9"															> Application.mk
echo "APP_STL				:= stlport_static"														>> Application.mk

pushd Core
echo "LOCAL_PATH := \$(call my-dir)" 																> Android.mk
echo 																								>> Android.mk
echo "include \$(CLEAR_VARS)" 																		>> Android.mk
echo "LOCAL_MODULE 			:= MonocleCore"															>> Android.mk
echo "LOCAL_SRC_FILES 		:= `find . -type f -iname '*.cpp' -printf %p\ `"						>> Android.mk
echo "LOCAL_C_INCLUDES 		:= \$(LOCAL_PATH)/../Libraries"											>> Android.mk #:= `find . -type d -printf \$\(LOCAL_PATH\)\/%f\ ` \\"					>> Android.mk
echo "LOCAL_CFLAGS 			:= -DMONOCLE_ANDROID -DMONOCLE_OPENGL_ES"								>> Android.mk
echo "LOCAL_STATIC_LIBRARIES:= tinyxml stbimage android_native_app_glue"							>> Android.mk
echo "LOCAL_LDLIBS 			:= -lGLESv1_CM -lEGL -lGLESv2 -lOpenSLES -landroid"						>> Android.mk
echo																								>> Android.mk
echo "include \$(BUILD_SHARED_LIBRARY)"																>> Android.mk
echo "\$(call import-module,android/native_app_glue)"												>> Android.mk
popd

pushd Libraries
echo "LOCAL_PATH := \$(call my-dir)" 																> Android.mk
echo 																								>> Android.mk
echo "include \$(CLEAR_VARS)" 																		>> Android.mk
echo "LOCAL_MODULE 			:= stbimage"															>> Android.mk
echo "LOCAL_SRC_FILES 		:= stb/stb_image.c"				 										>> Android.mk
echo "LOCAL_C_INCLUDES 		+= \$(LOCAL_PATH)/stb"													>> Android.mk
echo																								>> Android.mk
echo "include \$(BUILD_STATIC_LIBRARY)"																>> Android.mk

echo 																								>> Android.mk
echo "include \$(CLEAR_VARS)" 																		>> Android.mk
echo "LOCAL_MODULE 			:= tinyxml"																>> Android.mk
echo "LOCAL_SRC_FILES 		:= TinyXML/tinyxml.cpp TinyXML/tinyxmlerror.cpp \\"						>> Android.mk
echo "							TinyXML/tinyxmlparser.cpp"											>> Android.mk
echo "LOCAL_C_INCLUDES 		:= \$(LOCAL_PATH)/TinyXML \$(LOCAL_PATH)/../Core"						>> Android.mk
echo																								>> Android.mk
echo "include \$(BUILD_STATIC_LIBRARY)"																>> Android.mk
popd

popd


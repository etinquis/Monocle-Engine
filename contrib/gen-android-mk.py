from os import mkdir, listdir, walk, chdir, path
from shutil import copytree, rmtree

rmtree('../Build/gen-android-Monocle/jni/Core', ignore_errors=True)
rmtree('../Build/gen-android-Monocle/jni/Libraries', ignore_errors=True)

copytree('../Core', '../Build/gen-android-Monocle/jni/Core')
copytree('../Libraries', '../Build/gen-android-Monocle/jni/Libraries')

droidmk = open('../Build/gen-android-Monocle/jni/Android.mk', 'w')
droidmk.write('include $(call all-subdir-makefiles)\n')
droidmk.close()

droidmk = open('../Build/gen-android-Monocle/jni/Application.mk', 'w')
droidmk.write('APP_PLATFORM := android-9\n')
droidmk.write('APP_STL := stlport_static\n')

chdir('../Build/gen-android-Monocle/jni/Core')
droidmk = open('Android.mk', 'w')
droidmk.write('LOCAL_PATH 			:= $(call my-dir)\n')
droidmk.write('include $(CLEAR_VARS)\n')
droidmk.write('LOCAL_MODULE 		:= MonocleCore\n')
allfiles = []
curdir = ''
for root, dirs, files in walk('.'):
	allfiles += map(lambda name: path.join(root, name),files)
droidmk.write('LOCAL_SRC_FILES 		:= ' + ' '.join([file for file in allfiles if file.lower().endswith('.cpp')]) + '\n')
droidmk.write('LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/../Libraries\n')
droidmk.write('LOCAL_CFLAGS 			:= -DMONOCLE_ANDROID -DMONOCLE_OPENGL_ES\n')
droidmk.write('LOCAL_STATIC_LIBRARIES		:= tinyxml stbimage android_native_app_glue\n')
droidmk.write('LOCAL_LDLIBS 			:= -lGLESv1_CM -lEGL -lGLESv2 -lOpenSLES -landroid\n')
droidmk.write('include $(BUILD_SHARED_LIBRARY)\n')
droidmk.write('$(call import-module,android/native_app_glue)\n')
droidmk.close()

chdir('../Libraries')
droidmk = open('Android.mk', 'w')

droidmk.write('LOCAL_PATH := $(call my-dir)\n')
droidmk.write('include $(CLEAR_VARS)\n')
droidmk.write('LOCAL_MODULE 			:= stbimage\n')
droidmk.write('LOCAL_SRC_FILES 			:= stb/stb_image.c\n')
droidmk.write('LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/stb\n')
droidmk.write('include $(BUILD_STATIC_LIBRARY)\n')

droidmk.write('include $(CLEAR_VARS)\n')
droidmk.write('LOCAL_MODULE 			:= tinyxml\n')
droidmk.write('LOCAL_SRC_FILES 			:= TinyXML/tinyxml.cpp TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp\n')
droidmk.write('LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/TinyXML $(LOCAL_PATH)/../Core\n')
droidmk.write('include \$(BUILD_STATIC_LIBRARY)\n')

droidmk.close()

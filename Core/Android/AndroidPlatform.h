#ifdef MONOCLE_ANDROID

#pragma once

#include "../Platform.h"

#define ANDROID_POWERED

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Monocle", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Monocle", __VA_ARGS__))


namespace Monocle
{
	class AndroidPlatform
	{
	public:

		bool	active;									// Window Active Flag Set To TRUE By Default
		bool	fullscreen;								// Fullscreen Flag Set To Fullscreen Mode By Default

		static AndroidPlatform* instance;
		Platform *platform;

		struct android_app *state;
	
#ifdef ACCELEROMETER
		ASensorManager* sensorManager;
	    const ASensor* accelerometerSensor;
	    ASensorEventQueue* sensorEventQueue;
#endif
		int width;
		int height;

		EGLDisplay display;
	    EGLSurface surface;
	    EGLContext context;

		AndroidPlatform()
		{
			active = true;
			fullscreen = true;
			instance = this;
		}

		void WindowInit();
		static void KillPlatformWindow();
		static void GetDesktopSize(int *width, int *height);

	private:
	};

}

#endif

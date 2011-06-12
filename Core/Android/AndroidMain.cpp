#ifdef MONOCLE_ANDROID

#include "Monocle.h"

#include "AndroidPlatform.h"

#undef LOGI
#undef LOGW
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MonocleMain", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MonocleMain", __VA_ARGS__))

bool am_done;
int am_init = 0;

int engine_init_display(Monocle::AndroidPlatform *platform) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_RED_SIZE, 5,
			EGL_STENCIL_SIZE, EGL_DONT_CARE,
			EGL_DEPTH_SIZE, EGL_DONT_CARE,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config[20];
    EGLSurface surface;
    EGLContext context;
    LOGI("init_display");

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY) {
		LOGW("Display is none");
	}

	eglInitialize(display, 0, 0) == EGL_FALSE;
    LOGI("egl Initialized");

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, config, 20, &numConfigs);
	for (int i = 0; i < numConfigs; i++) {
		int r,g,b,a,d,s,st,f;
		eglGetConfigAttrib(display, config[i], EGL_RED_SIZE, &r);
		eglGetConfigAttrib(display, config[i], EGL_BLUE_SIZE, &b);
		eglGetConfigAttrib(display, config[i], EGL_GREEN_SIZE, &g);
		eglGetConfigAttrib(display, config[i], EGL_ALPHA_SIZE, &a);
		eglGetConfigAttrib(display, config[i], EGL_DEPTH_SIZE, &d);
		eglGetConfigAttrib(display, config[i], EGL_STENCIL_SIZE, &s);
		eglGetConfigAttrib(display, config[i], EGL_SURFACE_TYPE, &st);

	    eglGetConfigAttrib(display, config[i], EGL_NATIVE_VISUAL_ID, &f);

		if (a > 0 ) {
			LOGI("Config %d RGBA_%d%d%d%d s:%d d:%d st:%d f:%d",i,r,g,b,a,s,d,st,f);
		} else {
			LOGI("Config %d RGB_%d%d%d s:%d, d:%d st:%d f:%d",i,r,g,b,s,d,st,f);
		}
	}

	LOGI("egl returned %d configs", numConfigs);
    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */

	int conf_index = 0;
try_again:
    eglGetConfigAttrib(display, config[conf_index], EGL_NATIVE_VISUAL_ID, &format);
	LOGI("config %d egl format is %d",conf_index,format);
	
    //ANativeWindow_setBuffersGeometry(platform->state->window, 800, 480, format);

    surface = eglCreateWindowSurface(display, config[conf_index], platform->state->window, NULL);
	if (surface == EGL_NO_SURFACE) {
		LOGW("No surface %d with config", eglGetError(), conf_index);
		if (++conf_index < numConfigs) 
			goto try_again;
	}
    context = eglCreateContext(display, config[conf_index], NULL, NULL);
	if (context == EGL_NO_CONTEXT) {
		LOGW("No context %d", eglGetError());
	}

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	LOGI("Screen size %dx%d on surface %p", w, h, surface);
	((Monocle::Platform*)platform)->WindowSizeChanged(w, h);

    platform->display = display;
    platform->context = context;
    platform->surface = surface;
    platform->width = w;
    platform->height = h;

    // Initialize GL state.
	
	glDisable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);	

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glDisable(GL_DEPTH_TEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    return 0;
}

static void swapBuffers() {
	Monocle::AndroidPlatform *instance = Monocle::AndroidPlatform::instance;
	if (instance != NULL && instance->active ) {
		if (instance->display != EGL_NO_DISPLAY && instance->surface != EGL_NO_SURFACE) {
			unsigned int rc = eglSwapBuffers(instance->display, instance->surface);
			if (rc == EGL_FALSE) {
				int e = eglGetError();
				LOGW("eglSwapBuffers error %d (0x%x)",e, e);
				instance->KillPlatformWindow();
			}
		}
	}
}

/**
 *  * Tear down the EGL context currently associated with the display.
 *   */
void engine_term_display() {
	Monocle::AndroidPlatform *platform = Monocle::AndroidPlatform::instance;
	if (platform != NULL) {
		if (platform->display != EGL_NO_DISPLAY) {
			eglMakeCurrent(platform->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (platform->context != EGL_NO_CONTEXT) {
				eglDestroyContext(platform->display, platform->context);
			}
			if (platform->surface != EGL_NO_SURFACE) {
				eglDestroySurface(platform->display, platform->surface);
			}
			eglTerminate(platform->display);
		}
		platform->active = 0;
		platform->display = EGL_NO_DISPLAY;
		platform->context = EGL_NO_CONTEXT;
		platform->surface = EGL_NO_SURFACE;
	}
}
namespace Monocle {
	extern pthread_cond_t DisplayInitCond;

}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	LOGI("handle_cmd %d", cmd);
	Monocle::AndroidPlatform *platform = Monocle::AndroidPlatform::instance;
	if (platform) {
		platform->state = app;
	}
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
			LOGI("APP_CMD_SAVE_STATE");
            // The system has asked us to save our current state.  Do so.
			// TODO!
            break;
        case APP_CMD_INIT_WINDOW:
			LOGI("APP_CMD_INIT_WINDOW");
            // The window is being shown, get it ready.
            if (app->window != NULL) {
				pthread_cond_broadcast(&Monocle::DisplayInitCond);
				//engine_init_display(platform);
				platform->active = 1;
				LOGI("Window now active");
            }
            break;
		case APP_CMD_RESUME:
			LOGI("APP_CMD_RESUME");

			break;

        case APP_CMD_TERM_WINDOW:
			LOGI("APP_CMD_TERM_WINDOW");
            // The window is being hidden or closed, clean it up.
            engine_term_display();
            break;
        case APP_CMD_GAINED_FOCUS:
			LOGI("APP_CMD_GAINED_FOCUS");
            // When our app gains focus, we start monitoring the accelerometer.
			if (platform) {
				platform->active = 1;
			}
            break;
        case APP_CMD_LOST_FOCUS:
			// Pause
			LOGI("APP_CMD_LOST_FOCUS");
			if (platform) {
				platform->active = 0;
			}
            break;
    }
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	Monocle::AndroidPlatform *platform = Monocle::AndroidPlatform::instance;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		int x = AMotionEvent_getX(event, 0);
		int y = AMotionEvent_getY(event, 0);
		switch (AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(event)) {
			case AMOTION_EVENT_ACTION_DOWN: 		
				Monocle::Platform::mousePosition.x = x;
				Monocle::Platform::mousePosition.y = y;
				Monocle::Platform::SetMouseButton(Monocle::MOUSE_BUTTON_LEFT, true);
				LOGI("Touch down at (%d, %d)", (int)Monocle::Platform::mousePosition.x, (int)Monocle::Platform::mousePosition.y);
				break;

			case AMOTION_EVENT_ACTION_UP: 
				Monocle::Platform::mousePosition.x = x;
				Monocle::Platform::mousePosition.y = y;
				Monocle::Platform::SetMouseButton(Monocle::MOUSE_BUTTON_LEFT, false);
				LOGI("Touch up at (%d, %d)", (int)Monocle::Platform::mousePosition.x, (int)Monocle::Platform::mousePosition.y);
				break;

			case AMOTION_EVENT_ACTION_MOVE:
				Monocle::Platform::mousePosition.x = x;
				Monocle::Platform::mousePosition.y = y;
				LOGI("Touch still down at (%d, %d)", (int)Monocle::Platform::mousePosition.x, (int)Monocle::Platform::mousePosition.y);

				break;
		}
        return 1;
    }
    return 0;
}

int main(void);

void *main_thread(void *args) {
	//main();
	LOGI("Coming out of main?");
	am_done = true;
	pthread_exit(NULL);
}

void android_main(struct android_app* state) {
	pthread_t thread;
	app_dummy();
	
	am_done = false;

	LOGI("Up and running!");
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;

	pthread_create(&thread, NULL, main_thread, NULL);
	pthread_detach(thread);

	LOGI("Thread Created!");

	while (!am_init);

	Monocle::AndroidPlatform::instance->state = state;
	LOGI("Monocle Initialized proceding with android loading. %p", Monocle::AndroidPlatform::instance);

	while(!am_done) {
		int ident;
		int events;
		struct android_poll_source *source;

		if ((ident = ALooper_pollAll(1, NULL, &events, (void**)&source)) >= 0) {
			if (source != NULL) {
				source->process(state, source);
			}
			// Check if we are exiting.
			if (state->destroyRequested != 0) {
				engine_term_display();
				return;
			}

		}
	}

	return;
}

#endif

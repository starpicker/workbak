LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := RenderNV21

LOCAL_SRC_FILES  := RenderNV21.cpp \
					render.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc

LOCAL_LDLIBS := -llog \
				-lEGL -lGLESv2

LOCAL_CFLAGS := -Wno-write-strings

include $(BUILD_SHARED_LIBRARY)
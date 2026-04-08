// sherpa-onnx/jni/jni.cc
//
// Copyright (c)  2022-2023  Xiaomi Corporation
//                2022       Pingfeng Luo
//                2023       Zhaoming

#include <fstream>
#include <vector>

#include "sherpa-onnx/csrc/macros.h"
#include "sherpa-onnx/csrc/onnx-utils.h"
#include "sherpa-onnx/csrc/wave-writer.h"
#include "sherpa-onnx/jni/common.h"

// Global JavaVM pointer, cached in JNI_OnLoad.
// Needed to obtain a valid JNIEnv* on threads that were not created by the JVM
// (e.g., ONNX Runtime thread pool workers that invoke TTS callbacks).
static JavaVM *g_jvm = nullptr;

SHERPA_ONNX_EXTERN_C
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
  g_jvm = vm;
  return JNI_VERSION_1_6;
}

JavaVM *GetJavaVM() { return g_jvm; }

// see
// https://stackoverflow.com/questions/29043872/android-jni-return-multiple-variables
jobject NewInteger(JNIEnv *env, int32_t value) {
  jclass cls = env->FindClass("java/lang/Integer");
  jmethodID constructor = env->GetMethodID(cls, "<init>", "(I)V");
  jobject obj = env->NewObject(cls, constructor, value);
  env->DeleteLocalRef(cls);
  return obj;
}

jobject NewFloat(JNIEnv *env, float value) {
  jclass cls = env->FindClass("java/lang/Float");
  jmethodID constructor = env->GetMethodID(cls, "<init>", "(F)V");
  jobject obj = env->NewObject(cls, constructor, value);
  env->DeleteLocalRef(cls);
  return obj;
}

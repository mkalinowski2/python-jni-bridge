#pragma once

#include "App.h"
#include "JNIGameSupervisor.h"
#include <jni.h>
#include <string>

extern "C" {
JNIEXPORT void JNICALL Java_pl_fewbits_games_hardtimes_client_scripts_JNIScriptSupervisor_initialize(JNIEnv *, jobject, jobject gameSupervisor);
JNIEXPORT void JNICALL Java_pl_fewbits_games_hardtimes_client_scripts_JNIScriptSupervisor_finalize(JNIEnv *, jobject);
JNIEXPORT jobject JNICALL Java_pl_fewbits_games_hardtimes_client_scripts_JNIScriptSupervisor_sendRequest(JNIEnv *, jobject, jobject scriptRequest);
}

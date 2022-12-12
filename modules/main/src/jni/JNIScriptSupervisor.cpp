#include "JNIScriptSupervisor.h"

JNIEXPORT void JNICALL Java_pl_fewbits_games_hardtimes_client_scripts_JNIScriptSupervisor_initialize(JNIEnv *env, jobject, jobject gameSupervisor) {
  if (appScriptSupervisor != nullptr) {
    appScriptSupervisor->finalize();
    delete appScriptSupervisor;
    appScriptSupervisor = nullptr;
  }
  appScriptSupervisor = new ScriptSupervisor(new JNIGameSupervisor(env, gameSupervisor));
  appScriptSupervisor->initialize();
}

JNIEXPORT void JNICALL Java_pl_fewbits_games_hardtimes_client_scripts_JNIScriptSupervisor_finalize(JNIEnv *env, jobject) {
  if (appScriptSupervisor != nullptr) {
    appScriptSupervisor->finalize();
    delete appScriptSupervisor;
    appScriptSupervisor = nullptr;
  }
}

JNIEXPORT jobject JNICALL Java_pl_fewbits_games_hardtimes_client_scripts_JNIScriptSupervisor_sendRequest(JNIEnv *env, jobject, jobject javaScriptRequest) {
  jclass scriptResponseClass = env->FindClass("pl/fewbits/games/hardtimes/client/scripts/domain/JNIScriptResponse");
  jmethodID scriptResponseConstructorMethodId = env->GetMethodID(scriptResponseClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

  jclass javaScriptRequestClass = env->GetObjectClass(javaScriptRequest);
  jmethodID javaScriptRequestGetUrlMethodId = env->GetMethodID(javaScriptRequestClass, "getUrl", "()Ljava/lang/String;");
  jmethodID javaScriptRequestGetHeadersMethodId = env->GetMethodID(javaScriptRequestClass, "getHeaders", "()Ljava/lang/String;");
  jmethodID javaScriptRequestGetBodyMethodId = env->GetMethodID(javaScriptRequestClass, "getBody", "()Ljava/lang/String;");

  auto javaUrl = (jstring) env->CallObjectMethod(javaScriptRequest, javaScriptRequestGetUrlMethodId);
  auto javaHeaders = (jstring) env->CallObjectMethod(javaScriptRequest, javaScriptRequestGetHeadersMethodId);
  auto javaBody = (jstring) env->CallObjectMethod(javaScriptRequest, javaScriptRequestGetBodyMethodId);

  const char *convertedUrl = env->GetStringUTFChars(javaUrl, nullptr);
  const char *convertedHeaders = env->GetStringUTFChars(javaHeaders, nullptr);
  const char *convertedBody = env->GetStringUTFChars(javaBody, nullptr);

  auto url = std::string(convertedUrl);
  auto headers = std::string(convertedHeaders);
  auto body = std::string(convertedBody);

  env->ReleaseStringUTFChars(javaUrl, convertedUrl);
  env->ReleaseStringUTFChars(javaHeaders, convertedHeaders);
  env->ReleaseStringUTFChars(javaBody, convertedBody);

  auto scriptResponse = appScriptSupervisor->sendRequest(ScriptRequest(url, headers, body));
  return env->NewObject(
    scriptResponseClass,
    scriptResponseConstructorMethodId,
    env->NewStringUTF(scriptResponse.status.c_str()),
    env->NewStringUTF(scriptResponse.body.c_str())
  );
}

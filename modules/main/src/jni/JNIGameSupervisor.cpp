#include "JNIGameSupervisor.h"

JNIGameSupervisor::JNIGameSupervisor(JNIEnv *env, jobject &gameSupervisor) : env(env), gameSupervisor(gameSupervisor) {
  jclass gameSupervisorClass = env->GetObjectClass(gameSupervisor);
  jmethodID getPythonPathMethodId = env->GetMethodID(gameSupervisorClass, "getPythonDir", "()Ljava/lang/String;");
  jmethodID getPythonLibPathMethodId = env->GetMethodID(gameSupervisorClass, "getPythonLibDir", "()Ljava/lang/String;");
  jmethodID getGameHomeDirMethodId = env->GetMethodID(gameSupervisorClass, "getGameHomeDir", "()Ljava/lang/String;");

  auto javaPythonHomeDir = (jstring) env->CallObjectMethod(gameSupervisor, getPythonPathMethodId);
  auto javaPythonLibDir = (jstring) env->CallObjectMethod(gameSupervisor, getPythonLibPathMethodId);
  auto javaGameHomeDir = (jstring) env->CallObjectMethod(gameSupervisor, getGameHomeDirMethodId);

  const char *convertedPythonHomeDir = env->GetStringUTFChars(javaPythonHomeDir, nullptr);
  const char *convertedPythonLibDir = env->GetStringUTFChars(javaPythonLibDir, nullptr);
  const char *convertedGameHomeDir = env->GetStringUTFChars(javaGameHomeDir, nullptr);

  pythonHomeDir = std::string(convertedPythonHomeDir);
  pythonLibDir = std::string(convertedPythonLibDir);
  gameHomeDir = std::string(convertedGameHomeDir);

  env->ReleaseStringUTFChars(javaPythonHomeDir, convertedPythonHomeDir);
  env->ReleaseStringUTFChars(javaPythonLibDir, convertedPythonLibDir);
  env->ReleaseStringUTFChars(javaGameHomeDir, convertedGameHomeDir);
}

JNIGameSupervisor::~JNIGameSupervisor() = default;

std::string JNIGameSupervisor::getPythonDir() const {
  return pythonHomeDir;
}

std::string JNIGameSupervisor::getPythonLibDir() const {
    return pythonLibDir;
}

std::string JNIGameSupervisor::getGameHomeDir() const {
  return gameHomeDir;
}

int JNIGameSupervisor::generateNextInt(const std::string &rngType, const std::string &rngId) const {
  jclass gameSupervisorClass = env->GetObjectClass(gameSupervisor);
  jmethodID generateNextIntMethodId = env->GetMethodID(gameSupervisorClass, "generateNextInt", "(Ljava/lang/String;Ljava/lang/String;)I");
  return env->CallIntMethod(gameSupervisor, generateNextIntMethodId, env->NewStringUTF(rngType.c_str()), env->NewStringUTF(rngId.c_str()));
}

ScriptResponse JNIGameSupervisor::sendRequest(const ScriptRequest &scriptRequest) const {
  jclass javaScriptRequestClass = env->FindClass("pl/fewbits/games/hardtimes/client/scripts/domain/JNIScriptRequest");
  jmethodID javaScriptRequestConstructor = env->GetMethodID(
    javaScriptRequestClass,
    "<init>",
    "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"
  );
  jobject javaScriptRequest = env->NewObject(
    javaScriptRequestClass,
    javaScriptRequestConstructor,
    env->NewStringUTF(scriptRequest.url.c_str()),
    env->NewStringUTF(scriptRequest.headers.c_str()),
    env->NewStringUTF(scriptRequest.body.c_str())
  );

  jclass gameSupervisorClass = env->GetObjectClass(gameSupervisor);
  jmethodID sendRequestMethodId = env->GetMethodID(
    gameSupervisorClass,
    "sendRequest",
    "(Lpl/fewbits/games/hardtimes/client/scripts/domain/JNIScriptRequest;)Lpl/fewbits/games/hardtimes/client/scripts/domain/JNIScriptResponse;"
  );
  auto javaScriptResponse = env->CallObjectMethod(gameSupervisor, sendRequestMethodId, javaScriptRequest);

  jclass javaScriptResponseClass = env->GetObjectClass(javaScriptResponse);
  jmethodID javaScriptResponseGetStatusMethodId = env->GetMethodID(javaScriptResponseClass, "getStatus", "()Ljava/lang/String;");
  jmethodID javaScriptResponseGetBodyMethodId = env->GetMethodID(javaScriptResponseClass, "getBody", "()Ljava/lang/String;");

  auto javaStatus = (jstring) env->CallObjectMethod(javaScriptResponse, javaScriptResponseGetStatusMethodId);
  auto javaBody = (jstring) env->CallObjectMethod(javaScriptResponse, javaScriptResponseGetBodyMethodId);

  const char *convertedStatus = env->GetStringUTFChars(javaStatus, nullptr);
  const char *convertedBody = env->GetStringUTFChars(javaBody, nullptr);

  auto status = std::string(convertedStatus);
  auto body = std::string(convertedBody);

  env->ReleaseStringUTFChars(javaStatus, convertedStatus);
  env->ReleaseStringUTFChars(javaBody, convertedBody);

  return ScriptResponse(status, body);
}

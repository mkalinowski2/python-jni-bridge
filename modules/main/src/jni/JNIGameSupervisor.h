#pragma once

#include "GameSupervisor.h"
#include <jni.h>

class JNIGameSupervisor : public GameSupervisor {
  JNIEnv *const env;
  jobject gameSupervisor;

  std::string pythonHomeDir;
  std::string pythonLibDir;
  std::string gameHomeDir;

public:
  explicit JNIGameSupervisor(JNIEnv *env, jobject &gameSupervisor);
  ~JNIGameSupervisor() override;

  [[nodiscard]] std::string getPythonDir() const override;
  [[nodiscard]] std::string getPythonLibDir() const override;
  [[nodiscard]] std::string getGameHomeDir() const override;
  [[nodiscard]] int generateNextInt(const std::string &rngType, const std::string &rngId) const override;
  [[nodiscard]] ScriptResponse sendRequest(const ScriptRequest &scriptRequest) const override;
};

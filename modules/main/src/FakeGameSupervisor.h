#pragma once

#include "ScriptRequest.h"
#include "ScriptResponse.h"
#include "GameSupervisor.h"
#include <utility>

class FakeGameSupervisor : public GameSupervisor {
  std::string pythonHomeDir;
  std::string pythonLibDir;
  std::string gameHomeDir;

public:
  explicit FakeGameSupervisor(std::string pythonHomeDir, std::string pythonLibDir, std::string gameHomeDir);
  ~FakeGameSupervisor() override;

  [[nodiscard]] std::string getPythonDir() const override;
  [[nodiscard]] std::string getPythonLibDir() const override;
  [[nodiscard]] std::string getGameHomeDir() const override;
  [[nodiscard]] int generateNextInt(const std::string &rngType, const std::string &rngId) const override;
  [[nodiscard]] ScriptResponse sendRequest(const ScriptRequest &scriptRequest) const override;
};

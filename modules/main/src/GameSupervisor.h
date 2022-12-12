#pragma once

#include "ScriptRequest.h"
#include "ScriptResponse.h"

class GameSupervisor {
public:
  virtual ~GameSupervisor() = default;
  [[nodiscard]] virtual std::string getPythonDir() const = 0;
  [[nodiscard]] virtual std::string getPythonLibDir() const = 0;
  [[nodiscard]] virtual std::string getGameHomeDir() const = 0;
  [[nodiscard]] virtual int generateNextInt(const std::string &rngType, const std::string &rngId) const = 0;
  [[nodiscard]] virtual ScriptResponse sendRequest(const ScriptRequest &scriptRequest) const = 0;
};

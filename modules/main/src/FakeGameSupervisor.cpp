#include "FakeGameSupervisor.h"

FakeGameSupervisor::FakeGameSupervisor(
  std::string pythonHomeDir,
  std::string pythonLibDir,
  std::string gameHomeDir
) : pythonHomeDir(std::move(pythonHomeDir)), pythonLibDir(std::move(pythonLibDir)), gameHomeDir(std::move(gameHomeDir)) {}

FakeGameSupervisor::~FakeGameSupervisor() = default;

std::string FakeGameSupervisor::getPythonDir() const {
  return pythonHomeDir;
}

std::string FakeGameSupervisor::getPythonLibDir() const {
    return pythonLibDir;
}

std::string FakeGameSupervisor::getGameHomeDir() const {
  return gameHomeDir;
}

int FakeGameSupervisor::generateNextInt(const std::string &rngType, const std::string &rngId) const {
  return 2137;
}

ScriptResponse FakeGameSupervisor::sendRequest(const ScriptRequest &scriptRequest) const {
  return ScriptResponse("status", "body");
}

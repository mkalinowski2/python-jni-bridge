#pragma once

#include <string>
#include <utility>

struct ScriptResponse {
  std::string status;
  std::string body;

  ScriptResponse(std::string status, std::string body) : status(std::move(status)), body(std::move(body)) {}
};

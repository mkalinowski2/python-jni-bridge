#pragma once

#include <string>
#include <utility>

struct ScriptRequest {
  std::string url;
  std::string headers;
  std::string body;

  ScriptRequest(std::string url, std::string headers, std::string body) : url(std::move(url)), headers(std::move(headers)), body(std::move(body)) {}
};

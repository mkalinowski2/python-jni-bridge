#pragma once

#include "GameSupervisor.h"
#include <string>
#include <sstream>
#include <Python.h>

class ScriptSupervisor {
  PyObject *pythonAppModule{};
  PyObject *pythonExecuteRequestFunction{};
public:

  GameSupervisor *const gameSupervisor;
  explicit ScriptSupervisor(GameSupervisor *gameSupervisor);
  ~ScriptSupervisor();

  void initialize();
  void finalize();
  [[nodiscard]] ScriptResponse sendRequest(const ScriptRequest &scriptRequest) const;

  PyObject* getPythonDirFunForPython(PyObject *self, PyObject *args);
  PyObject* getGameHomeDirFunForPython(PyObject *self, PyObject *args);
  PyObject* generateNextIntFunForPython(PyObject *self, PyObject *args);
  PyObject* sendRequestFunForPython(PyObject *self, PyObject *args);
};


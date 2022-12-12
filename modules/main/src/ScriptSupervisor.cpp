#include "ScriptSupervisor.h"
#include "Platform.h"

WINDOWS_DLLEXPORT
ScriptSupervisor *appScriptSupervisor = nullptr;

static PyObject *getPythonDirNative(PyObject * self, PyObject * args) {
  return appScriptSupervisor->getPythonDirFunForPython(self, args);
}

static PyObject *getPythonDylibDirNative(PyObject * self, PyObject * args) {
  return appScriptSupervisor->getPythonDirFunForPython(self, args);
}

static PyObject *getGameHomeDirNative(PyObject * self, PyObject * args) {
  return appScriptSupervisor->getGameHomeDirFunForPython(self, args);
}

static PyObject *generateNextIntNative(PyObject * self, PyObject * args) {
  return appScriptSupervisor->generateNextIntFunForPython(self, args);
}

static PyObject *sendRequestNative(PyObject * self, PyObject * args) {
  return appScriptSupervisor->sendRequestFunForPython(self, args);
}

static PyMethodDef NativeCoreMethods[] = {
  {"getPythonDirNative", getPythonDirNative, METH_VARARGS, "Return Python dir"},
  {"getPythonDylibDirNative", getPythonDylibDirNative, METH_VARARGS, "Return Python dylib dir"},
  {"getGameHomeDirNative", getGameHomeDirNative, METH_VARARGS, "Return GameHome dir"},
  {"generateNextIntNative", generateNextIntNative, METH_VARARGS, "Return random integer"},
  {"sendRequestNative", sendRequestNative, METH_VARARGS, "Return response from request"},
  {NULL, NULL, 0, NULL}
};

static PyModuleDef NativeCoreModule = {
  PyModuleDef_HEAD_INIT,
  "nativeCore",
  nullptr,
  -1,
  NativeCoreMethods,
  nullptr,
  nullptr,
  nullptr,
  nullptr
};

static PyObject *PyInit_NativeCoreModule() {
  return PyModule_Create(&NativeCoreModule);
}

ScriptSupervisor::ScriptSupervisor(GameSupervisor *gameSupervisor) : gameSupervisor(gameSupervisor) {}

ScriptSupervisor::~ScriptSupervisor() {
  delete gameSupervisor;
}

void ScriptSupervisor::initialize() {
  loadPythonLibraryGlobally(gameSupervisor->getPythonLibDir().c_str());

  std::wstringstream pythonHomePathStream;
  pythonHomePathStream << gameSupervisor->getPythonDir().c_str();

  std::wstringstream pythonPathStream;
  pythonPathStream << gameSupervisor->getPythonDir().c_str() << L"" STRINGIFY(PLATFORM_PATH_SEPARATOR) ""
    << gameSupervisor->getPythonDir().c_str() << L"" STRINGIFY(PLATFORM_DIR_SEPARATOR) ""  << "python" << L"" STRINGIFY(PLATFORM_PATH_SEPARATOR) ""
    << gameSupervisor->getPythonLibDir().c_str() << L"" STRINGIFY(PLATFORM_DIR_SEPARATOR) "" << "lib-dynload" << L"" STRINGIFY(PLATFORM_PATH_SEPARATOR) ""
    << gameSupervisor->getPythonDir().c_str() << L"" STRINGIFY(PLATFORM_DIR_SEPARATOR) "" << "site-packages" << L"" STRINGIFY(PLATFORM_PATH_SEPARATOR) ""
    << gameSupervisor->getGameHomeDir().c_str() << L"" STRINGIFY(PLATFORM_DIR_SEPARATOR) "" << L"" STRINGIFY(PLATFORM_PATH_SEPARATOR) ""
    << gameSupervisor->getGameHomeDir().c_str() << L"" STRINGIFY(PLATFORM_DIR_SEPARATOR) "" << "src";



  Py_SetPythonHome(pythonHomePathStream.str().c_str());
  Py_SetPath(pythonPathStream.str().c_str());
  Py_SetProgramName(L"GameScriptApp");

  setvbuf(stdout, nullptr, PLATFORM_SETVBUF_TYPE, 0);

  PyImport_AppendInittab("nativeCore", &PyInit_NativeCoreModule);
  Py_InitializeEx(0);
  PyObject * pythonModuleName = PyUnicode_DecodeFSDefault("gameScriptApp");
  pythonAppModule = PyImport_Import(pythonModuleName);
  Py_DECREF(pythonModuleName);
  if (pythonAppModule == nullptr) {
    PyErr_Print();
    std::exit(1);
  }
  pythonExecuteRequestFunction = PyObject_GetAttrString((PyObject *) pythonAppModule, "executeRequest");

  PyObject * pythonInitializeFunction = PyObject_GetAttrString((PyObject *) pythonAppModule, "initialize");
  PyObject_CallObject(pythonInitializeFunction, nullptr);

  Py_DECREF(pythonInitializeFunction);
}

void ScriptSupervisor::finalize() {
  if (pythonExecuteRequestFunction != nullptr) {
    Py_DECREF((PyObject *) pythonExecuteRequestFunction);
    pythonExecuteRequestFunction = nullptr;
  }
  if (pythonAppModule != nullptr) {
    Py_DECREF((PyObject *) pythonAppModule);
    pythonAppModule = nullptr;
    Py_Finalize();
  }
}

ScriptResponse ScriptSupervisor::sendRequest(const ScriptRequest &scriptRequest) const {
  PyObject * pythonArgs = PyTuple_New(3);
  PyTuple_SetItem(pythonArgs, 0, PyUnicode_FromString(scriptRequest.url.c_str()));
  PyTuple_SetItem(pythonArgs, 1, PyUnicode_FromString(scriptRequest.headers.c_str()));
  PyTuple_SetItem(pythonArgs, 2, PyUnicode_FromString(scriptRequest.body.c_str()));
  PyObject * pythonValue = PyObject_CallObject((PyObject *) pythonExecuteRequestFunction, pythonArgs);
  if (pythonValue == nullptr) {
    PyErr_Print();
    std::exit(1);
  }

  PyObject * pythonStatus = PyList_GetItem(pythonValue, 0);
  PyObject * pythonBody = PyList_GetItem(pythonValue, 1);
  auto status = std::string(PyUnicode_AsUTF8(pythonStatus));
  auto body = std::string(PyUnicode_AsUTF8(pythonBody));

  Py_DECREF(pythonArgs);
  Py_DECREF(pythonValue);

  return ScriptResponse(status, body);
}

PyObject *ScriptSupervisor::getPythonDirFunForPython(PyObject *self, PyObject *args) {
  return PyUnicode_FromString(gameSupervisor->getPythonDir().c_str());
}

PyObject *ScriptSupervisor::getGameHomeDirFunForPython(PyObject *self, PyObject *args) {
  return PyUnicode_FromString(gameSupervisor->getGameHomeDir().c_str());
}

PyObject *ScriptSupervisor::generateNextIntFunForPython(PyObject *self, PyObject *args) {
  char *rngType, *rngId;
  PyArg_ParseTuple(args, "ss", &rngType, &rngId);
  auto nextInt = gameSupervisor->generateNextInt(std::string(rngType), std::string(rngId));
  return PyLong_FromLong(nextInt);
}

PyObject *ScriptSupervisor::sendRequestFunForPython(PyObject *self, PyObject *args) {
  char *url, *body, *headers;
  PyArg_ParseTuple(args, "sss", &url, &body, &headers);
  auto scriptResponse = gameSupervisor->sendRequest(
    ScriptRequest(
      std::string(url),
      std::string(body),
      std::string(headers))
  );
  return Py_BuildValue("[ss]", scriptResponse.status.c_str(), scriptResponse.body.c_str());
}

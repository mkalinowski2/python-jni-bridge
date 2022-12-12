#include "ScriptSupervisor.h"
#include "FakeGameSupervisor.h"
#include "Platform.h"
#include "iostream"

WINDOWS_DLLIMPORT
extern ScriptSupervisor* appScriptSupervisor;

#if defined(OS_WIN)
#define PYTHON_HOME_DIR "D:\\workspace\\hardtime-client-core-native-sources\\hardtimes-client-core-platform\\python"
#define PYTHON_LIB_DIR "D:\\workspace\\hardtime-client-core-native-sources\\hardtimes-client-core-platform\\python\\lib\\windows-x64"
#define GAME_HOME_DIR "D:\\workspace\\hardtimes-client-core\\hardtimes-client-core-resources"
#else
#define PYTHON_HOME_DIR "/home/ignus/internal/hardtime-client-core-native-sources/hardtimes-client-core-platform/python"
#define PYTHON_LIB_DIR "/home/ignus/internal/hardtime-client-core-native-sources/hardtimes-client-core-platform/python/lib/linux-x64"
#define GAME_HOME_DIR "/home/ignus/internal/hardtimes-client-core/hardtimes-client-core-resources"
#endif

int main(int argc, char *argv[]) {
  GameSupervisor *gameSupervisor = new FakeGameSupervisor(
    PYTHON_HOME_DIR,
    PYTHON_LIB_DIR,
    GAME_HOME_DIR
  );
  appScriptSupervisor = new ScriptSupervisor(gameSupervisor);
  appScriptSupervisor->initialize();
  auto response = appScriptSupervisor->sendRequest(
    ScriptRequest(
      std::string("getUnitStatsOnBattleStart"),
      std::string("headers"),
      std::string(
        R"({"unitType":"Angel","customAttributes":{"playerId":"1","unitId":"25d4aee8-2a8f-4275-a79f-b08b2202803a","isTurnToRightSide":"true","isTurnToRightSideByDefault":"true","hasActionToPerform":"true","hasAdditionalActionFromGoodMorale":"false","wasWaitActionPerformed":"false","numberUnitsInStack":"1","position":"0;0;0"}})")
    )
  );
  std::cout << response.body;
  appScriptSupervisor->finalize();
  delete appScriptSupervisor;
  return 0;
}

// curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
// python3.8 get-pip.py
// python3.8 -m pip install python-math

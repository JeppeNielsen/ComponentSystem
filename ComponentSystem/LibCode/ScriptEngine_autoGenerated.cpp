#include "libCode.h"
#include "ScriptTest.hpp"

extern "C" int CountComponents() {
   return 1;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 0: return new Button();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((Button*)component); break; }
   }
}

extern "C" int CountSystem() {
   return 1;
}
extern "C" IScriptSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new ButtonSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IScriptSystem* scriptSystem) {
   delete scriptSystem; 
}
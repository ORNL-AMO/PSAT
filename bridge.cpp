#include <node.h>
#include "../api/Calculator/PumpEfficiency.h"
#include "../api/Calculator/MotorRatedPower.h"

using v8::FunctionCallbackInfo;
using v8::Local;
using v8::Object;
using v8::Value;
// using v8::Isolate;
// using v8::String;

void Method(const FunctionCallbackInfo<Value>& args) {
  auto *p = new PumpEfficiency();
  double d = args[0]->NumberValue();
  args.GetReturnValue().Set(p->calculate());
}

void Method2(const FunctionCallbackInfo<Value>& args) {
  auto *m = new MotorRatedPower(123);
  args.GetReturnValue().Set(m->calculate());
}

void results(const FunctionCallbackInfo<Value>& args) {
  auto *p = new PumpEfficiency();
  double d = args[0]->NumberValue();
  args.GetReturnValue().Set(p->calculate());
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "pumpEff", Method);
  NODE_SET_METHOD(exports, "mrp", Method2);
  NODE_SET_METHOD(exports, "results", results);    
}

NODE_MODULE(bridge, init)


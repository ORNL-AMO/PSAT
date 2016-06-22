#include <node.h>
#include "../api/Calculator/PumpEfficiency.h"
// #include "../api/Calculator/PumpEfficiency.h"

using v8::FunctionCallbackInfo;
using v8::Local;
using v8::Object;
using v8::Value;
// using v8::Isolate;
// using v8::String;

void Method(const FunctionCallbackInfo<Value>& args) {
  auto *p = new PumpEfficiency();
  double d = args[0]->NumberValue();
  args.GetReturnValue().Set(p->calculate(d));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "pumpEff", Method);
}

NODE_MODULE(bridge, init)


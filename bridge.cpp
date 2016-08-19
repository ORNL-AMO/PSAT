#include <node.h>
#include "../api/Calculator/PumpEfficiency.h"
#include "../api/Calculator/MotorRatedPower.h"

using namespace v8;

void Method(const FunctionCallbackInfo<Value>& args) {
  auto *p = new PumpEfficiency();
  double d = args[0]->NumberValue();
  args.GetReturnValue().Set(p->calculate());
}

void Method2(const FunctionCallbackInfo<Value>& args) {
  auto *m = new MotorRatedPower(123);
  args.GetReturnValue().Set(m->calculate());
}

void Results(const FunctionCallbackInfo<Value>& args) {
  Isolate* iso = args.GetIsolate();
  Local<Array> r = Array::New(iso);
  r->Set(0,Number::New(iso,99));
  auto *p = new PumpEfficiency();
  double d = args[0]->NumberValue();
  //double r[] = {123,456,789};
  args.GetReturnValue().Set(r);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "pumpEff", Method);
  NODE_SET_METHOD(exports, "mrp", Method2);
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, init)


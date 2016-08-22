#include <node.h>
#include "../api/Calculator/PumpEfficiency.h"
#include "../api/Calculator/MotorRatedPower.h"

using namespace v8;

void Results(const FunctionCallbackInfo<Value>& args) {
  Isolate* iso = args.GetIsolate();
  Local<Array> r = Array::New(iso);

  auto *p = new PumpEfficiency(0,0,0,0);
  r->Set(0,Number::New(iso,p->calculate()));
  r->Set(1,Number::New(iso,99));
  
  auto *m = new MotorRatedPower(123);
  r->Set(2,Number::New(iso,m->calculate()));
  r->Set(3,Number::New(iso,91));
  
  r->Set(4,Number::New(iso,234));
  r->Set(5,Number::New(iso,432));

  r->Set(6,Number::New(iso,345));
  r->Set(7,Number::New(iso,76.24));
  
  args.GetReturnValue().Set(r);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, init)


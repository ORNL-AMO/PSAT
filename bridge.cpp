#include <node.h>
#include "../api/Calculator/PumpEfficiency.h"
#include "../api/Calculator/MotorRatedPower.h"

using namespace v8;

Local<Array> r;
Isolate* iso;

void set(double v1, double v2) {
  r->Set(r->Length(),Number::New(iso,v1));
  r->Set(r->Length(),Number::New(iso,v2));
}
void Results(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  r = Array::New(iso);

  auto *p = new PumpEfficiency(0,0,0,0);
  set(p->calculate(),99);
  
  auto *m = new MotorRatedPower(123);
  set(m->calculate(),91);

  set(234,432); 
  set(666,667);

  args.GetReturnValue().Set(r);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, init)


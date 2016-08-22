#include <node.h>
#include "../api//Pump.h"
#include "../api/Calculator/PumpEfficiency.h"
#include "../api/Calculator/OptimalPumpEfficiency.h"
#include "../api/Calculator/MotorRatedPower.h"
#include "../api/Calculator/OptimalMotorRatedPower.h"



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

  set((new PumpEfficiency(0,0,0,0))->calculate(),
    (new OptimalPumpEfficiency(Pump::Style::END_SUCTION_SLURRY,0,0,0,0,0,Pump::Speed::FIXED_SPECIFIC_SPEED))->calculate());
  
  set((new MotorRatedPower(0))->calculate(),(new OptimalMotorRatedPower(0,0))->calculate());

  set(234,432); 
  set(666,667);

  args.GetReturnValue().Set(r);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, init)


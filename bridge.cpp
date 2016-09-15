#include <node.h>
#include <initializer_list>

#include "Pump.h"
#include "calculator/PumpEfficiency.h"
#include "calculator/OptimalPumpEfficiency.h"
#include "calculator/MotorRatedPower.h"
#include "calculator/OptimalMotorRatedPower.h"
#include "calculator/MotorShaftPower.h"
#include "calculator/OptimalMotorShaftPower.h"
#include "calculator/PumpShaftPower.h"
#include "calculator/OptimalPumpShaftPower.h"
#include "calculator/MotorEfficiency.h"
#include "calculator/OptimalMotorEfficiency.h"
#include "calculator/MotorPowerFactor.h"
#include "calculator/OptimalMotorPowerFactor.h"
#include "calculator/MotorCurrent.h"
#include "calculator/OptimalMotorCurrent.h"
#include "calculator/MotorPower.h"
#include "calculator/OptimalMotorPower.h"
#include "AnnualEnergy.h"
#include "AnnualCost.h"
#include "AnnualSavingsPotential.h"
#include "OptimizationRating.h"

using namespace v8;

Local<Array> r;
Isolate* iso;
Local<Object> inp;

void SetR(std::initializer_list <double> args) {
  for (auto d : args) {
    r->Set(r->Length(),Number::New(iso,d));
  }
}
double Get(const char *nm) {
  return inp->ToObject()->Get(String::NewFromUtf8(iso,nm))->NumberValue();
}
void Results(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  r = Array::New(iso);
  inp = args[0]->ToObject();
  
  auto drive = static_cast<Pump::Drive>(Get("drive"));
  auto effCls = static_cast<Motor::EfficiencyClass>(Get("efficiency_class"));

  auto loadMeth = FieldData::LoadEstimationMethod::POWER;
  double mp = Get("motor_field_power");
  double mc = Get("motor_field_current");
  if (mc>0) {
    loadMeth = FieldData::LoadEstimationMethod::CURRENT;
    mp = (new MotorPower(Get("motor_rated_voltage"),mc,0))->calculate();//power factor
  } else {
    mc = (new MotorCurrent(Get("motor_rated_power"),Get("motor_rated_speed"),effCls,0))->calculate();//loadf
  }
  SetR({
    (new PumpEfficiency(Get("specific_gravity"),Get("flow"),Get("head"),0))->calculate(),//pumpShaftPower
    (new OptimalPumpEfficiency(static_cast<Pump::Style>(Get("style")),
      Get("pump_rated_speed"),Get("viscosity"),Get("stages"),Get("flow"),Get("head"),static_cast<Pump::Speed>(!Get("speed"))))->calculate(),//
    mp,
    (new OptimalMotorRatedPower(0,Get("margin")))->calculate(),//motorshaftpower
    (new MotorShaftPower(Get("motor_rated_power"),mp,Get("motor_rated_speed"),effCls,Get("motor_rated_voltage")))->calculate(),
    (new OptimalMotorShaftPower(0,drive))->calculate(),//pumpshaftpower
    (new PumpShaftPower(0,drive))->calculate(),//motorshaftpower
    (new OptimalPumpShaftPower(Get("flow"),Get("head"),Get("specific_gravity"),0))->calculate(),//pumpeff
    (new MotorEfficiency(Get("motor_rated_speed"),effCls,Get("motor_rated_power"),mp,0))->calculate(),//loadF
    (new OptimalMotorEfficiency(Get("motor_rated_power"),0))->calculate(),//motor shaft power
    (new MotorPowerFactor(Get("motor_rated_power"),0,mc,0,Get("motor_rated_voltage")))->calculate(),//loadFactor??, motor eff
    (new OptimalMotorPowerFactor(Get("motor_rated_power"),0))->calculate(),//opt motor power?
    mc,
    (new OptimalMotorCurrent(0,Get("field_voltage")))->calculate(),//opt motor power
    mp,
    (new OptimalMotorPower(0,0))->calculate(),//motorshaftpower, motor eff
    (new AnnualEnergy(mp,Get("fraction")))->calculate(),//motorpower
    (new AnnualEnergy(0,Get("fraction")))->calculate(),//opt motorpower
    (new AnnualCost(0,Get("cost")))->calculate(),//ex ann energy
    (new AnnualCost(0,Get("cost")))->calculate(),//opt ann energy
    -1,
    (new AnnualSavingsPotential(0,0))->calculate(),//ex an cost, opt an cost
    -1,
    (new OptimizationRating(0,0))->calculate()//ex an cost, opt an cost
  });
  args.GetReturnValue().Set(r);
}
void EstFLA(const FunctionCallbackInfo<Value>& args) {
  args[0]->NumberValue();//hp
  args[1]->NumberValue();//speed
  args[2]->NumberValue();//v
  args[3]->NumberValue();//cls
  args[4]->NumberValue();//cus cls
  args.GetReturnValue().Set(args[4]);
}
void Curve(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  r = Array::New(iso);
  SetR({args[0]->NumberValue(),args[1]->NumberValue(),args[2]->NumberValue(),args[3]->NumberValue()});
  args.GetReturnValue().Set(r);
}
void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);
  NODE_SET_METHOD(exports, "estFLA", EstFLA);    
  NODE_SET_METHOD(exports, "curve", Curve);    
}

NODE_MODULE(bridge, Init)


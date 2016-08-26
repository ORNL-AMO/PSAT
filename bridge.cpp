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

void set(std::initializer_list <double> args) {
  for (auto d : args) {
    r->Set(r->Length(),Number::New(iso,d));
  }
}
double get(const char *nm) {
  return inp->ToObject()->Get(String::NewFromUtf8(iso,nm))->NumberValue();
}
void Results(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  r = Array::New(iso);
  inp = args[0]->ToObject();
  
  auto drive = static_cast<Pump::Drive>(get("drive"));
  auto effCls = static_cast<Motor::EfficiencyClass>(get("efficiency_class"));

  auto loadMeth = FieldData::LoadEstimationMethod::POWER;
  double mp = get("motor_field_power");
  double mc = get("motor_field_current");
  if (mc>0) {
    loadMeth = FieldData::LoadEstimationMethod::CURRENT;
    mp = (new MotorPower(0,mc,0,get("field_voltage")))->calculate();//motor power (makes no sense, it IS motor power), motor a, motorpf
  } else {
    mc = (new MotorCurrent(0,mp,get("field_voltage")))->calculate();//motor a, motor power, recursive arg!!
  }
  set({
    (new PumpEfficiency(get("specific_gravity"),get("flow"),get("head"),0))->calculate(),//pumpShaftPower
    (new OptimalPumpEfficiency(static_cast<Pump::Style>(get("style")),
      get("pump_rated_speed"),get("viscosity"),get("stages"),get("flow"),get("head"),static_cast<Pump::Speed>(!get("speed"))))->calculate(),//
    get("motor_rated_power"),
    (new OptimalMotorRatedPower(0,get("margin")))->calculate(),//motorshaftpower
    (new MotorShaftPower(0,mp))->calculate(),//motor eff 
    (new OptimalMotorShaftPower(0,drive))->calculate(),//pumpshaftpower
    (new PumpShaftPower(0,drive))->calculate(),//motorshaftpower
    (new OptimalPumpShaftPower(get("flow"),get("head"),get("specific_gravity"),0))->calculate(),//pumpeff
    (new MotorEfficiency(get("line"),get("motor_rated_speed"),effCls,get("motor_rated_power"),
      loadMeth,0,mc,get("field_voltage")))->calculate(),//motorKwh??
    (new OptimalMotorEfficiency(get("motor_rated_power"),0))->calculate(),//motor shaft power
    (new MotorPowerFactor(get("line"),get("rpm"),effCls,get("power_rating"),
      loadMeth,0,mc,get("field_voltage")))->calculate(),//motor kwh
    (new OptimalMotorPowerFactor(get("motor_rated_power"),0))->calculate(),//opt motor power?
    mc,
    (new OptimalMotorCurrent(0,get("field_voltage")))->calculate(),//opt motor power
    mp,
    (new OptimalMotorPower(0,0))->calculate(),//motorshaftpower, motor eff
    (new AnnualEnergy(mp,get("fraction")))->calculate(),//motorpower
    (new AnnualEnergy(0,get("fraction")))->calculate(),//opt motorpower
    (new AnnualCost(0,get("cost")))->calculate(),//ex ann energy
    (new AnnualCost(0,get("cost")))->calculate(),//opt ann energy
    -1,
    (new AnnualSavingsPotential(0,0))->calculate(),//ex an cost, opt an cost
    -1,
    (new OptimizationRating(0,0))->calculate()//ex an cost, opt an cost
  });
  args.GetReturnValue().Set(r);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, init)


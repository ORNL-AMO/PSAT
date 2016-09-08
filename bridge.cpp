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

void SetArgs(std::initializer_list <double> args) {
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
    mp = (new MotorPower(0,mc,0,Get("field_voltage")))->calculate();//motor power (makes no sense, it IS motor power), motor a, motorpf
  } else {
    mc = (new MotorCurrent(Get("motor_rated_power"),mp,Get("motor_rated_speed"),effCls)/*Get("field_voltage")*/)->calculate();//motor a, motor power, recursive arg!!
  }
  SetArgs({
    (new PumpEfficiency(Get("specific_gravity"),Get("flow"),Get("head"),0))->calculate(),//pumpShaftPower
    (new OptimalPumpEfficiency(static_cast<Pump::Style>(Get("style")),
      Get("pump_rated_speed"),Get("viscosity"),Get("stages"),Get("flow"),Get("head"),static_cast<Pump::Speed>(!Get("speed"))))->calculate(),//
    mp,
    (new OptimalMotorRatedPower(0,Get("margin")))->calculate(),//motorshaftpower
    (new MotorShaftPower(0,mp,Get("motor_rated_speed"),effCls))->calculate(),//motor eff 
    (new OptimalMotorShaftPower(0,drive))->calculate(),//pumpshaftpower
    (new PumpShaftPower(0,drive))->calculate(),//motorshaftpower
    (new OptimalPumpShaftPower(Get("flow"),Get("head"),Get("specific_gravity"),0))->calculate(),//pumpeff
    (new MotorEfficiency(Get("line"),Get("motor_rated_speed"),effCls,Get("motor_rated_power"),
      loadMeth,0,mc,Get("field_voltage"),Get("motor_rated_power"),Get("motor_field_power")))->calculate(),//motorKwh?? mp instead of field power? what about measured current case?
    (new OptimalMotorEfficiency(Get("motor_rated_power"),0))->calculate(),//motor shaft power
    (new MotorPowerFactor(Get("line"),Get("rpm"),effCls,Get("power_rating"),
      loadMeth,0,mc,Get("field_voltage")))->calculate(),//motor kwh
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
    (new OptimizationRating(0,0))->calculate(),//ex an cost, opt an cost

    777//est fla; hp,class,v,speed
  });
  args.GetReturnValue().Set(r);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, Init)


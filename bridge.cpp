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
double get(const char * nm) {
  return inp->ToObject()->Get(String::NewFromUtf8(iso,nm))->NumberValue();
}
void Results(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  r = Array::New(iso);
  inp = args[0]->ToObject();

  set({
    (new PumpEfficiency(get("specific_gravity"),get("flow"),get("head"),0))->calculate(),
    (new OptimalPumpEfficiency(static_cast<Pump::Style>(get("style")),get("pump_rpm"),get("viscosity"),get("stages"),get("flow"),get("head"),static_cast<Pump::Speed>(!get("speed"))))->calculate(),
    (new MotorRatedPower(get("power_rating")))->calculate(),
    (new OptimalMotorRatedPower(0,get("margin")))->calculate(),
    (new MotorShaftPower(0,0))->calculate(), 
    (new OptimalMotorShaftPower(0,Pump::Drive::DIRECT_DRIVE))->calculate(),
    (new PumpShaftPower(0,static_cast<Pump::Drive>(get("drive"))))->calculate(),
    (new OptimalPumpShaftPower(get("flow"),get("head"),get("specific_gravity"),0))->calculate(),
    (new MotorEfficiency(get("line"),get("rpm"),static_cast<Motor::EfficiencyClass>(get("efficiency_class")),get("power_rating"),static_cast<FieldData::LoadEstimationMethod>(get("load_method")),0,0,get("field_voltage")))->calculate(),
    (new OptimalMotorEfficiency(0,0))->calculate(),
    (new MotorPowerFactor(get("line"),get("rpm"),static_cast<Motor::EfficiencyClass>(get("efficiency_class")),get("power_rating"),static_cast<FieldData::LoadEstimationMethod>(get("load_method")),0,0,get("field_voltage")))->calculate(),
    (new OptimalMotorPowerFactor(0,0))->calculate(),
    (new MotorCurrent(0,0,get("field_voltage")))->calculate(),
    (new OptimalMotorCurrent(0,get("field_voltage")))->calculate(),
    (new MotorPower(0,0,0,get("field_voltage")))->calculate(),
    (new OptimalMotorPower(0,0))->calculate(),
    (new AnnualEnergy(0,get("fraction")))->calculate(),
    (new AnnualEnergy(0,get("fraction")))->calculate(),
    (new AnnualCost(0,0))->calculate(),
    (new AnnualCost(0,0))->calculate(),
    -1,
    (new AnnualSavingsPotential(0,0))->calculate(),
    -1,
    (new OptimizationRating(0,0))->calculate()
  });
  args.GetReturnValue().Set(r);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);    
}

NODE_MODULE(bridge, init)


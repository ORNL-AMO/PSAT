#include <node.h>
#include <initializer_list>

#include "../amo-library/Pump.h"
#include "../amo-library/calculator/PumpEfficiency.h"
#include "../amo-library/calculator/OptimalPumpEfficiency.h"
#include "../amo-library/calculator/MotorRatedPower.h"
#include "../amo-library/calculator/OptimalMotorRatedPower.h"
#include "../amo-library/calculator/MotorShaftPower.h"
#include "../amo-library/calculator/OptimalMotorShaftPower.h"
#include "../amo-library/calculator/PumpShaftPower.h"
#include "../amo-library/calculator/OptimalPumpShaftPower.h"
#include "../amo-library/calculator/MotorEfficiency.h"
#include "../amo-library/calculator/OptimalMotorEfficiency.h"
#include "../amo-library/calculator/MotorPowerFactor.h"
#include "../amo-library/calculator/OptimalMotorPowerFactor.h"
#include "../amo-library/calculator/MotorCurrent.h"
#include "../amo-library/calculator/OptimalMotorCurrent.h"
#include "../amo-library/calculator/MotorPower.h"
#include "../amo-library/calculator/OptimalMotorPower.h"
#include "../amo-library/AnnualEnergy.h"
#include "../amo-library/AnnualCost.h"
#include "../amo-library/AnnualSavingsPotential.h"
#include "../amo-library/OptimizationRating.h"

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
    (new PumpShaftPower(0,static_cast<Pump::Drive>(get("drive"))))))->calculate(),
    (new OptimalPumpShaftPower(0,0,0,0))->calculate(),
    (new MotorEfficiency(0,0,Motor::EfficiencyClass::STANDARD,0,FieldData::LoadEstimationMethod::POWER,0,0,0))->calculate(),
    (new OptimalMotorEfficiency(0,0))->calculate(),
    (new MotorPowerFactor(0,0,Motor::EfficiencyClass::STANDARD,0,FieldData::LoadEstimationMethod::POWER,0,0,0))->calculate(),
    (new OptimalMotorPowerFactor(0,0))->calculate(),
    (new MotorCurrent(0,0,0))->calculate(),
    (new OptimalMotorCurrent(0,0))->calculate(),
    (new MotorPower(0,0,0,0))->calculate(),
    (new OptimalMotorPower(0,0))->calculate(),
    (new AnnualEnergy(0,0))->calculate(),
    (new AnnualEnergy(0,0))->calculate(),
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


#include <node.h>
#include <initializer_list>

#include "../api/Pump.h"
#include "../api/Calculator/PumpEfficiency.h"
#include "../api/Calculator/OptimalPumpEfficiency.h"
#include "../api/Calculator/MotorRatedPower.h"
#include "../api/Calculator/OptimalMotorRatedPower.h"
#include "../api/Calculator/MotorShaftPower.h"
#include "../api/Calculator/OptimalMotorShaftPower.h"
#include "../api/Calculator/PumpShaftPower.h"
#include "../api/Calculator/OptimalPumpShaftPower.h"
#include "../api/Calculator/MotorEfficiency.h"
#include "../api/Calculator/OptimalMotorEfficiency.h"
#include "../api/Calculator/MotorPowerFactor.h"
#include "../api/Calculator/OptimalMotorPowerFactor.h"
#include "../api/Calculator/MotorCurrent.h"
#include "../api/Calculator/OptimalMotorCurrent.h"
#include "../api/Calculator/MotorPower.h"
#include "../api/Calculator/OptimalMotorPower.h"
#include "../api/AnnualEnergy.h"
#include "../api/AnnualCost.h"
#include "../api/AnnualSavingsPotential.h"
#include "../api//OptimizationRating.h"

using namespace v8;

Local<Array> r;
Isolate* iso;

void set(std::initializer_list <double> args) {
  for (auto d : args) {
    r->Set(r->Length(),Number::New(iso,d));
  }
}
void Results(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  r = Array::New(iso);
  set({
    (new PumpEfficiency(0,0,0,0))->calculate(),
    (new OptimalPumpEfficiency(Pump::Style::END_SUCTION_SLURRY,0,0,0,0,0,Pump::Speed::FIXED_SPECIFIC_SPEED))->calculate(),
    (new MotorRatedPower(0))->calculate(),
    (new OptimalMotorRatedPower(0,0))->calculate(),
    (new MotorShaftPower(0,0))->calculate(), 
    (new OptimalMotorShaftPower(0,Pump::Drive::DIRECT_DRIVE))->calculate(),
    (new PumpShaftPower(0,Pump::Drive::DIRECT_DRIVE))->calculate(),
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


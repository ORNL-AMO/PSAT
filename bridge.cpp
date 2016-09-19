#include <node.h>
#include <initializer_list>
#include <map>
#include <vector>

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
  auto r = Object::New(iso);
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
  auto msp = (new MotorShaftPower(Get("motor_rated_power"),mp,Get("motor_rated_speed"),effCls,Get("motor_rated_voltage")));
  std::map<const char *, std::vector<double>> m = { // nested list-initialization
    {"Motor Shaft Power", {msp->calculate(),0}},
    {"Motor Current", {msp->calculateCurrent(),0}},
    {"Motor Efficiency", {msp->calculateEfficiency(),0}},
    {"Motor Power Factor", {msp->calculatePowerFactor(),0}},
    {"Motor Power", {msp->calculateElectricPower(),0}},
    {"I0-1",{Get("motor_rated_power"),mp}},
     {"I2-3",{Get("motor_rated_speed"),Get("efficiency_class")}},
    {"I4",{Get("motor_rated_voltage"),0}}
  };    
  
  for(auto p: m) {    
    auto a = Array::New(iso);
    a->Set(0,Number::New(iso,p.second[0]));
    a->Set(1,Number::New(iso,p.second[1]));          
    r->Set(String::NewFromUtf8(iso,p.first),a);
  }
  // SetR({
  //   0,//(new PumpEfficiency(Get("specific_gravity"),Get("flow"),Get("head"),0))->calculate(),//pumpShaftPower
  //   0,//(new OptimalPumpEfficiency(static_cast<Pump::Style>(Get("style")),
  //     //Get("pump_rated_speed"),Get("viscosity"),Get("stages"),Get("flow"),Get("head"),static_cast<Pump::Speed>(!Get("speed"))))->calculate(),
  //   0,//mp,
  //   0,//(new OptimalMotorRatedPower(0,Get("margin")))->calculate(),//motorshaftpower
  //   (new MotorShaftPower(Get("motor_rated_power"),mp,Get("motor_rated_speed"),effCls,Get("motor_rated_voltage")))->calculate(),
  //   0,//(new OptimalMotorShaftPower(0,drive))->calculate(),//pumpshaftpower
  //   0,//(new PumpShaftPower(0,drive))->calculate(),//motorshaftpower
  //   0,//(new OptimalPumpShaftPower(Get("flow"),Get("head"),Get("specific_gravity"),0))->calculate(),//pumpeff
  //   0,//(new MotorEfficiency(Get("motor_rated_speed"),effCls,Get("motor_rated_power"),mp,0))->calculate(),//loadF
  //   0,//(new OptimalMotorEfficiency(Get("motor_rated_power"),0))->calculate(),//motor shaft power
  //   0,//(new MotorPowerFactor(Get("motor_rated_power"),0,mc,0,Get("motor_rated_voltage")))->calculate(),//loadFactor??, motor eff
  //   0,//(new OptimalMotorPowerFactor(Get("motor_rated_power"),0))->calculate(),//opt motor power?
  //   0,//mc,
  //   0,//(new OptimalMotorCurrent(0,Get("field_voltage")))->calculate(),//opt motor power
  //   0,//mp,
  //   0,//(new OptimalMotorPower(0,0))->calculate(),//motorshaftpower, motor eff
  //   0,//(new AnnualEnergy(mp,Get("fraction")))->calculate(),//motorpower
  //   0,//(new AnnualEnergy(0,Get("fraction")))->calculate(),//opt motorpower
  //   0,//(new AnnualCost(0,Get("cost")))->calculate(),//ex ann energy
  //   0,//(new AnnualCost(0,Get("cost")))->calculate(),//opt ann energy
  //   -1,
  //   0,//(new AnnualSavingsPotential(0,0))->calculate(),//ex an cost, opt an cost
  //   -1,
  //   0,//(new OptimizationRating(0,0))->calculate()//ex an cost, opt an cost
  // });
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


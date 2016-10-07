#include <iostream>
#include <vector>
#include <map>
#include <node.h>

#include "PSATResult.h"
#include "calculator/EstimateFLA.h"

using namespace v8;
using namespace std;

Isolate* iso;
Local<Object> inp;

//todo assert exist
double Get(const char *nm) {
  return inp->ToObject()->Get(String::NewFromUtf8(iso,nm))->NumberValue();
}

void Results(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  inp = args[0]->ToObject();
  auto r = Object::New(iso);

  auto drive = (Pump::Drive)(int)Get("drive");
  auto effCls = (Motor::EfficiencyClass)(int)Get("efficiency_class");
 
  Pump pump((Pump::Style)(int)Get("pump_style"),Get("pump_rated_speed"),drive,
      Get("viscosity"),Get("specific_gravity"),Get("stages"),(Pump::Speed)(int)(!Get("fixed_speed")));
  Motor motor((Motor::LineFrequency)(int)(!Get("line")),Get("motor_rated_power"),Get("motor_rated_speed"),
      effCls,Get("efficiency"),Get("motor_rated_voltage"),Get("motor_rated_flc"),Get("margin"));
  Financial fin(Get("fraction"),Get("cost"));
  FieldData fd(Get("flow"),Get("head"),(FieldData::LoadEstimationMethod)(Get("motor_field_power")>0?0:1),
      Get("motor_field_power"),Get("motor_field_current"),Get("motor_field_voltage"));
  PSATResult psat(pump,motor,fin,fd);
  psat.calculateExisting();
  auto ex = psat.getExisting();
  
  map<const char *,vector<double>> out = { 
    {"Pump Efficiency",{ex.pumpEfficiency_*100,0}},
    {"Motor Rated Power",{ex.motorRatedPower_,0}},        
    {"Motor Shaft Power",{ex.motorShaftPower_,0}},
    {"Pump Shaft Power",{ex.pumpShaftPower_,0}},    
    {"Motor Efficiency",{ex.motorEfficiency_,0}},
    {"Motor Power Factor",{ex.motorPowerFactor_,0}},
    {"Motor Current",{ex.motorCurrent_,0}},    
    {"Motor Power", {ex.motorPower_,0}},
    {"Annual Energy", {ex.annualEnergy_,0}},
    {"Annual Cost", {ex.annualCost_*1000,0}},
    {"Savings Potential", {psat.getAnnualSavingsPotential(),0}},
    {"Optimization Rating", {psat.getOptimizationRating(),0}}
  };
  for(auto p: out) {    
    auto a = Array::New(iso);
    a->Set(0,Number::New(iso,p.second[0]));
    a->Set(1,Number::New(iso,p.second[1]));          
    r->Set(String::NewFromUtf8(iso,p.first),a);
  }
  args.GetReturnValue().Set(r);
}

void EstFLA(const FunctionCallbackInfo<Value>& args) {
  iso = args.GetIsolate();
  inp = args[0]->ToObject();

  EstimateFLA fla(Get("motor_rated_power"),Get("motor_rated_speed"),(Motor::EfficiencyClass)(int)Get("efficiency_class"),
    Get("efficiency"),Get("motor_rated_voltage"));
  fla.calculate();
  args.GetReturnValue().Set(fla.getEstimatedFLA());
}

void Check(double exp, double act, const char* nm="") {
  cout << "e " << exp << "; a " << act << endl;
  if (abs(exp-act)>.1*exp) {
    printf("\"%s\" TEST FAILED: %f %f\n",nm,exp,act);
    assert(!"equal");
  }
}

void Check100(double exp, double act, const char* nm="") {
  Check(exp,act*100,nm);
}

void TestSame() {
  // auto msp = (new MotorShaftPower(200,80,1786,Motor::EfficiencyClass::ENERGY_EFFICIENT,460,460));
  for (int i=1; i<=10000; i=i+2) {
    // Check(msp->calculate(),msp->calculate(),"SAME");
  }
}
// void Test(const FunctionCallbackInfo<Value>& args) {
//   TestSame();
//   //assume power load meth, est fla, line=60
//   auto msp = new MotorShaftPower(200,80,1786,Motor::EfficiencyClass::ENERGY_EFFICIENT,460,460);
//   Check(101.9,msp->calculate());
//   Check100(95,msp->calculateEfficiency());
//   Check100(79.1,msp->calculatePowerFactor());
//   Check(127,msp->calculateCurrent());

//   msp = new MotorShaftPower(200,111.855,1780,Motor::EfficiencyClass::ENERGY_EFFICIENT,460,460);
//   Check(143.4,msp->calculate());
//   Check100(95.6,msp->calculateEfficiency());
//   Check100(84.3,msp->calculatePowerFactor());
//   Check(166.5,msp->calculateCurrent());

//   msp = new MotorShaftPower(200,80,1780,Motor::EfficiencyClass::ENERGY_EFFICIENT,460,260);
//   Check(101.9,msp->calculate());
//   Check100(95,msp->calculateEfficiency());
//   Check100(138.8,msp->calculatePowerFactor());
//   Check(128,msp->calculateCurrent());

//   msp = new MotorShaftPower(100,80,1780,Motor::EfficiencyClass::ENERGY_EFFICIENT,460,460);
//   Check(101.8,msp->calculate());
//   Check100(94.9,msp->calculateEfficiency());
//   Check100(86.7,msp->calculatePowerFactor());
//   Check(115.8,msp->calculateCurrent());

//   msp = new MotorShaftPower(200,80,1200,Motor::EfficiencyClass::ENERGY_EFFICIENT,460,460);
//   Check(101.4,msp->calculate());
//   Check100(94.5,msp->calculateEfficiency());
//   Check100(74.3,msp->calculatePowerFactor());
//   Check(135.1,msp->calculateCurrent());
  
//   // msp = new MotorShaftPower(200,80,1780,Motor::EfficiencyClass::ENERGY_EFFICIENT,200,460);
//   // Check(101.9,msp->calculate());
//   // Check100(95,msp->calculateEfficiency());
//   // Check100(35.2,msp->calculatePowerFactor());
//   // Check(285,msp->calculateCurrent());

//   auto ae = (new AnnualEnergy(80,1))->calculate();
//   Check(700.8,ae);

//   ae = (new AnnualEnergy(150,.25))->calculate();
//   Check(328.5,ae);
  
//   auto ac = (new AnnualCost(328.5,.05))->calculate();
//   Check(16.4,ac);

// }

void TestFLA(const FunctionCallbackInfo<Value>& args) {
  EstimateFLA fla(200,1780,(Motor::EfficiencyClass)(1),0,460);
  fla.calculate();
  Check(225.8,fla.getEstimatedFLA());
}
void Test3(const FunctionCallbackInfo<Value>& args) {
  #define BASE \
    Pump pump(Pump::Style::END_SUCTION_ANSI_API,1780,Pump::Drive::DIRECT_DRIVE,\
      1,1,1,Pump::Speed::NOT_FIXED_SPEED);\
    Motor motor(Motor::LineFrequency::FREQ60,200,1780,\
        Motor::EfficiencyClass::ENERGY_EFFICIENT,0,460,225.8,0);\
    Financial fin(1,.05);\
    FieldData fd(2000,277,FieldData::LoadEstimationMethod::POWER,\
        150,0,460); 

  #define CALC \
    PSATResult psat(pump,motor,fin,fd);\
    psat.calculateExisting();\
    auto ex = psat.getExisting();

  {
    BASE
    motor.setMotorRpm(1786);
    fd.setMotorPower(80);
    CALC
    Check(101.9,ex.motorShaftPower_);
    Check100(95,ex.motorEfficiency_);
    Check100(79.1,ex.motorPowerFactor_);
    Check(127,ex.motorCurrent_);
  }
  {
    BASE
    CALC
    Check(217.1,ex.motorCurrent_);
  }
  {
    BASE    
    fd.setLoadEstimationMethod(FieldData::LoadEstimationMethod::CURRENT);
    fd.setMotorAmps(218);
    fd.setMotorPower(0);
    CALC
    Check(150.7,ex.motorPower_);
    Check100(72.5,ex.pumpEfficiency_);
  }
}
void Wtf(const FunctionCallbackInfo<Value>& args) {
  Pump pump(Pump::Style::END_SUCTION_ANSI_API,1780,Pump::Drive::DIRECT_DRIVE,
      1,1,1,Pump::Speed::NOT_FIXED_SPEED);
  Motor motor(Motor::LineFrequency::FREQ60,200,1786,
      Motor::EfficiencyClass::ENERGY_EFFICIENT,0,460,225.8,0);
  Financial fin(1,.05);
  FieldData fd(2000,277,FieldData::LoadEstimationMethod::POWER,
      80,0,460);   

  PSATResult psat(pump,motor,fin,fd);
  psat.calculateExisting();
  auto ex = psat.getExisting();

  cout << "msp " << ex.motorShaftPower_ << endl;
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "results", Results);
  NODE_SET_METHOD(exports, "estFLA", EstFLA); 
  NODE_SET_METHOD(exports, "test", Wtf);  
}

NODE_MODULE(bridge, Init)


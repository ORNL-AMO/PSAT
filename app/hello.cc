#include <node.h>
#include <iostream>
#include <vector>
#include "Calculator.h"

using namespace std;

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

int myTest(void);
double wtf(void);

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "worldX"));
  //std::cout << wtf() << std::endl;	

  Calculator *c = new Calculator();

	int a,b;
	a=3.0; b=10.0;    
    
	//cout << a << endl;
	//cout << b << endl;
    
	//cout << c->testD() << endl;



// vector<int> v = {7, 5, 16, 8};
 
//     // Add two more integers to vector
//     v.push_back(25);
//     v.push_back(13);
 
//     // Iterate and print values of vector
//     for(int n : v) {
//         cout << n << '\n';
//     }
       
// }

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(addon, init)


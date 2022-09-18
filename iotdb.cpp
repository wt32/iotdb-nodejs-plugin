#include <node.h>

namespace demo {

	using v8::Context;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;
	using v8::Array;
	using v8::Number;

	void CreateObject(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Local<Context> context = isolate->GetCurrentContext();

		Local<Object> obj = Object::New(isolate);
		obj->Set(context,String::NewFromUtf8(isolate,"msg").ToLocalChecked(),args[0]->ToString(context).ToLocalChecked())
			.FromJust();

		args.GetReturnValue().Set(obj);
	}

	void CreateArray(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Local<Context> context = isolate->GetCurrentContext();

		Local<Array> arr = Array::New(isolate);
		Local<Object> obj = Object::New(isolate);
		obj->Set(context, String::NewFromUtf8(isolate, "key").ToLocalChecked(), String::NewFromUtf8(isolate, "value").ToLocalChecked())
			.FromJust();
		arr->Set(context, 0, obj);
		arr->Set(context, 3, String::NewFromUtf8(isolate, "a2a").ToLocalChecked());
		arr->Set(context, 4, String::NewFromUtf8(isolate, "a4a").ToLocalChecked());
		
		args.GetReturnValue().Set(arr);
	}

	void Init(Local<Object> exports, Local<Object> module) {
		NODE_SET_METHOD(exports, "obj", CreateObject);
		NODE_SET_METHOD(exports, "array", CreateArray);
		
	}

	NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo

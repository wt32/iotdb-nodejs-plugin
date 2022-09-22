#include "Session.h"
#include <napi.h>
using namespace std;
Session *session;

#define CHECK_NAPI(...)                                                        \
  do {                                                                         \
    napi_status res__ = (__VA_ARGS__);                                         \
    if (res__ != napi_ok) {                                                    \
      return res__;                                                            \
    }                                                                          \
  } while (0)

class Iotdb : public Napi::ObjectWrap<Iotdb> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Iotdb(const Napi::CallbackInfo &info);
    static napi_status ToUtf8String(napi_env env,
                                  napi_value value,
                                  string* result);

  private:
    static Napi::FunctionReference constructor;
    double _value;
    Napi::Value GetValue(const Napi::CallbackInfo &info);
    void SetValue(const Napi::CallbackInfo &info,const Napi::Value &value);
    Napi::Value Query(const Napi::CallbackInfo &info);
    Napi::Value Demo(const Napi::CallbackInfo &info);

};

Napi::Object Iotdb::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Iotdb", {
        // InstanceAccessor("value", &Iotdb::GetValue, &Iotdb::SetValue),
        // InstanceAccessor("readOnlyProp", &Iotdb::GetValue, nullptr),
        InstanceMethod("query", &Iotdb::Query),
        InstanceMethod("Demo", &Iotdb::Demo)
    });


    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Iotdb", func);

    return exports;
}

Iotdb::Iotdb(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Iotdb>(info) {
    Napi::Env env = info.Env();
    string ip;
    int port;
    string username;
    string password;
    ToUtf8String(env, info[0].As<Napi::String>(), &ip);
    napi_get_value_int32(env,info[1].As<Napi::Number>() , &port);
    ToUtf8String(env, info[2].As<Napi::String>(), &username);
    ToUtf8String(env, info[3].As<Napi::String>(), &password);

    session = new Session(ip, port, username, password);
    session->open(false);
    // Napi::String value = info[0].As<Napi::String>();
    // this->_value = value.DoubleValue();
}


napi_status Iotdb::ToUtf8String(napi_env env,
                                           napi_value value,
                                           string* result) {
  size_t size{};
  CHECK_NAPI(napi_get_value_string_utf8(env, value, nullptr, 0, &size));
  result->assign(size, ' ');
  return napi_get_value_string_utf8(
      env, value, &(*result)[0], size + 1, nullptr);
}



Napi::FunctionReference Iotdb::constructor;

Napi::Value Iotdb::GetValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->_value);
}

void Iotdb::SetValue(const Napi::CallbackInfo &info, const Napi::Value &value) {
    // Napi::Env env = info.Env();
    // ...
    Napi::Number arg = value.As<Napi::Number>();
    this->_value = arg.DoubleValue();
}


Napi::Value Iotdb::Query(const Napi::CallbackInfo &info){
    Napi::Env env = info.Env();
    std::string sql;
    ToUtf8String(env, info[0].As<Napi::String>(), &sql);
    unique_ptr<SessionDataSet> dataSet = session->executeQueryStatement(sql);
    for (const string &name: dataSet->getColumnNames()) {
        cout << "\"" << name << "\"  ";
    }
    cout << endl;

    dataSet->setBatchSize(1024);
    while (dataSet->hasNext()) {
        cout << dataSet->next()->toString();
    }
    cout << endl;

    dataSet->closeOperationHandle();
    return Napi::String::New(env, "Hello World");
}

Napi::Value Iotdb::Demo(const Napi::CallbackInfo &info){
    session->close();
    delete session;
   return info[0].As<Napi::String>();
}


// Initialize native add-on
Napi::Object Init (Napi::Env env, Napi::Object exports) {
    Iotdb::Init(env, exports);
    return exports;
}



// Register and initialize native add-on
NODE_API_MODULE(hello_world, Init)
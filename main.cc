#include <leveldb/c.h>
#include <string.h>
#include <nan.h>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

leveldb_t *db;
leveldb_options_t *options;
leveldb_readoptions_t *roptions;
leveldb_writeoptions_t *woptions;
char *err = NULL;
char *read;
size_t read_len;

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "string conversion failed";
}

void Open(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong type of arguments");
    return;
  }

  String::Utf8Value str(info[0]);
  const char* arg = ToCString(str);

  if(strlen(arg) == 0) {
    Nan::ThrowError("Database name is empty");
    return;
  } else {
    options = leveldb_options_create();
    leveldb_options_set_create_if_missing(options, 1);

    // compression is currently disabled because it's causing problems
    // leveldb_options_set_compression(options, 2);

    db = leveldb_open(options, arg, &err);

    // this is broken for some reason, so it's disabled for now
    // if (err != NULL) {
    //   Nan::ThrowError("Failed to open database");
    //   return;
    // }
    // leveldb_free(err); err = NULL;

    leveldb_options_destroy(options);
    info.GetReturnValue().Set(Nan::New("opened").ToLocalChecked());
  }
}

void Close(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  leveldb_close(db);
  info.GetReturnValue().Set(Nan::New("closed").ToLocalChecked());
}

void Get(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() && !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  String::Utf8Value str(info[0]);
  const char* arg = ToCString(str);

  roptions = leveldb_readoptions_create();
  read = leveldb_get(db, roptions, arg, info[1]->NumberValue(), &read_len, &err);

  // this is broken for some reason, so it's disabled for now
  // if (err != NULL) {
  //   Nan::ThrowError("Read fail");
  //   return;
  // }
  // leveldb_free(err); err = NULL;

  leveldb_readoptions_destroy(roptions);
  info.GetReturnValue().Set(Nan::New(read).ToLocalChecked());
}

void Put(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() != 4) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() && !info[1]->IsNumber() && !info[2]->IsString() && !info[3]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  String::Utf8Value str(info[0]);
  const char* arg = ToCString(str);

  String::Utf8Value str2(info[2]);
  const char* arg2 = ToCString(str2);

  woptions = leveldb_writeoptions_create();
  leveldb_put(db, woptions, arg, info[1]->NumberValue(), arg2, info[3]->NumberValue(), &err);

  // this is broken for some reason, so it's disabled for now
  // if (err != NULL) {
  //   Nan::ThrowError("Write fail");
  //   return;
  // }
  // leveldb_free(err); err = NULL;

  leveldb_writeoptions_destroy(woptions);
  info.GetReturnValue().Set(Nan::New("written").ToLocalChecked());
}

void Delete(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() && !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  String::Utf8Value str(info[0]);
  const char* arg = ToCString(str);

  woptions = leveldb_writeoptions_create();
  leveldb_delete(db, woptions, arg, info[1]->NumberValue(), &err);

  // this is broken for some reason, so it's disabled for now
  // if (err != NULL) {
  //   Nan::ThrowError("Delete fail");
  //   return;
  // }
  // leveldb_free(err); err = NULL;

  info.GetReturnValue().Set(Nan::New("deleted").ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("open").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(Open)->GetFunction());
  exports->Set(Nan::New("close").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(Close)->GetFunction());
  exports->Set(Nan::New("get").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(Get)->GetFunction());
  exports->Set(Nan::New("put").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(Put)->GetFunction());
  exports->Set(Nan::New("delete").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(Delete)->GetFunction());
}

NODE_MODULE(addon, Init)

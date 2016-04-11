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

namespace addon {
  leveldb_t *db;

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
      leveldb_options_t *options;
      options = leveldb_options_create();
      leveldb_options_set_create_if_missing(options, 1);

      leveldb_options_set_compression(options, 2);

      char *err = NULL;
      db = leveldb_open(options, arg, &err);

      if (err != NULL) {
        Nan::ThrowError("Failed to open database");
        return;
      }
      leveldb_free(err); err = NULL;

      leveldb_options_destroy(options);
      info.GetReturnValue().Set(Nan::New("opened").ToLocalChecked());
    }
  }

  void Close(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    leveldb_close(db);
    info.GetReturnValue().Set(Nan::New("closed").ToLocalChecked());
  }

  void Get(const Nan::FunctionCallbackInfo<v8::Value>& info) {

    if (info.Length() != 1) {
     Nan::ThrowTypeError("Wrong number of arguments");
     return;
    }

    if (!info[0]->IsString()) {
     Nan::ThrowTypeError("Wrong arguments");
     return;
    }

    String::Utf8Value str(info[0]);
    const char* arg = ToCString(str);

    leveldb_readoptions_t *roptions;
    roptions = leveldb_readoptions_create();

    char *read;
    size_t read_len;
    char *err = NULL;
    read = leveldb_get(db, roptions, arg, str.length(), &read_len, &err);

    if (err != NULL) {
      Nan::ThrowError("Read fail");
      return;
    }
    leveldb_free(err); err = NULL;

    leveldb_readoptions_destroy(roptions);
    info.GetReturnValue().Set(Nan::New(read,read_len).ToLocalChecked());
  }

  void Put(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() != 2) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    if (!info[0]->IsString() && !info[1]->IsString()) {
      Nan::ThrowTypeError("Wrong arguments");
      return;
    }

    String::Utf8Value str(info[0]);
    const char* arg = ToCString(str);

    String::Utf8Value str2(info[1]);
    const char* arg2 = ToCString(str2);

    leveldb_writeoptions_t *woptions;
    woptions = leveldb_writeoptions_create();
    char *err = NULL;
    leveldb_put(db, woptions, arg, str.length(), arg2, str2.length(), &err);

    if (err != NULL) {
      Nan::ThrowError("Write fail");
      return;
    }
    leveldb_free(err); err = NULL;

    leveldb_writeoptions_destroy(woptions);
    info.GetReturnValue().Set(Nan::New("written").ToLocalChecked());
  }

  void Delete(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    if (!info[0]->IsString()) {
      Nan::ThrowTypeError("Wrong arguments");
      return;
    }

    String::Utf8Value str(info[0]);
    const char* arg = ToCString(str);

    leveldb_writeoptions_t *woptions;
    woptions = leveldb_writeoptions_create();
    char *err = NULL;
    leveldb_delete(db, woptions, arg, str.length(), &err);

    if (err != NULL) {
      Nan::ThrowError("Delete fail");
      return;
    }
    leveldb_free(err); err = NULL;

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
}

#include <node.h>
#include <leveldb/c.h>
#include <string.h>

namespace demo {

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

void Open(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  options = leveldb_options_create();
  leveldb_options_set_create_if_missing(options, 1);
  leveldb_options_set_compression(options, 2);
  String::Utf8Value str(args[0]);
  const char* bar = ToCString(str);
  db = leveldb_open(options, bar, &err);

  if (err != NULL) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Failed to open database")));
    return;
  }

  leveldb_free(err); err = NULL;
  leveldb_options_destroy(options);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "opened"));
}

void Close(const FunctionCallbackInfo<Value>& args) {
  leveldb_close(db);
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "closed"));
}

void Get(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  roptions = leveldb_readoptions_create();
  //leveldb_options_set_compression(roptions, leveldb_zlib_compression);
  String::Utf8Value str(args[0]);
  const char* bar = ToCString(str);

  read = leveldb_get(db, roptions, bar, args[1]->NumberValue(), &read_len, &err);

  if (err != NULL) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Read fail")));
    return;
  }

  leveldb_free(err); err = NULL;
  leveldb_readoptions_destroy(roptions);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, read));
}

void Put(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  String::Utf8Value str(args[0]);
  const char* bar = ToCString(str);

  String::Utf8Value str2(args[2]);
  const char* bar2 = ToCString(str2);

  woptions = leveldb_writeoptions_create();
  //leveldb_options_set_compression(woptions, leveldb_zlib_compression);
  leveldb_put(db, woptions, bar, args[1]->NumberValue(), bar2, args[3]->NumberValue(), &err);

  if (err != NULL) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Write fail")));
    return;
  }

  leveldb_free(err); err = NULL;
  leveldb_writeoptions_destroy(woptions);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "written"));
}

void Delete(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  String::Utf8Value str(args[0]);
  const char* bar = ToCString(str);

  leveldb_delete(db, woptions, bar, args[1]->NumberValue(), &err);

  if (err != NULL) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Delete fail")));
    return;
  }

  leveldb_free(err); err = NULL;
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "deleted"));
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "open", Open);
  NODE_SET_METHOD(exports, "close", Close);
  NODE_SET_METHOD(exports, "get", Get);
  NODE_SET_METHOD(exports, "put", Put);
  NODE_SET_METHOD(exports, "delete", Delete);
}

NODE_MODULE(addon, Init)

}

#include <leveldb/c.h>
#include <nan.h>
#include <cstring>
#include <map>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

const char* ToCString(const String::Utf8Value& value) {
  if (!*value) {
    Nan::ThrowError("Failed convert argument to string");
  }

  return *value;
}

namespace node_leveldb_mcpe_native {
class Database : public Nan::ObjectWrap {
public:
  leveldb_t* db_;
  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Database").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "close", Close);
    Nan::SetPrototypeMethod(tpl, "get", Get);
    Nan::SetPrototypeMethod(tpl, "put", Put);
    Nan::SetPrototypeMethod(tpl, "delete", Delete);

    // FIXME(keegan), this should be an embedded class
    Nan::SetPrototypeMethod(tpl, "iteratorNew", IterNew);
    Nan::SetPrototypeMethod(tpl, "iteratorNext", IterNext);
    Nan::SetPrototypeMethod(tpl, "iteratorValid", IterValid);
    Nan::SetPrototypeMethod(tpl, "iteratorDestroy", IterDestroy);
    Nan::SetPrototypeMethod(tpl, "iteratorKey", IterKey);
    Nan::SetPrototypeMethod(tpl, "iteratorValue", IterValue);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Database").ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked());
  }

private:
  uint32_t iterator_count_ = 0;

  // TODO(keegan), replace this with a class
  std::map<uint32_t, leveldb_iterator_t*> iterators_;
  std::map<uint32_t, leveldb_readoptions_t*> iterator_options_;

  explicit Database(leveldb_t* db) : db_(db) {}
  ~Database() {}

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      if (info.Length() != 1) {
        Nan::ThrowTypeError("Incorrect number of arguments; `name` required");
        return;
      }

      if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Incorrect type of `name`, string was expected");
        return;
      }

      String::Utf8Value str(info[0]); // `name`
      const char* arg = ToCString(str);

      if (strlen(arg) == 0) {
        Nan::ThrowError("`name` required");
        return;
      } else {
        leveldb_options_t* options;
        options = leveldb_options_create();
        leveldb_options_set_create_if_missing(options, 1);

        leveldb_options_set_compression(options, 2);

        char* err = NULL;
        leveldb_t* db = leveldb_open(options, arg, &err);

        if (err != NULL) {
          Nan::ThrowError("Failed to open database");
          return;
        }
        leveldb_free(err);
        err = NULL;

        leveldb_options_destroy(options);

        Database* instance = new Database(db);
        instance->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
      }
    }
  }

  static NAN_METHOD(Close) {
    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());
    leveldb_close(instance->db_);
    info.GetReturnValue().Set(Nan::Undefined());
  }

  static NAN_METHOD(Get) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Incorrect number of arguments; `key` required");
      return;
    }

    if (!node::Buffer::HasInstance(info[0])) {
      Nan::ThrowTypeError("Incorrect type of `key`, Buffer was expected");
      return;
    }

    Local<Object> input = info[0]->ToObject();
    const char* key_input = node::Buffer::Data(input);
    size_t key_input_sz = node::Buffer::Length(input);

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());
    leveldb_t* db = instance->db_;

    leveldb_readoptions_t* roptions;
    roptions = leveldb_readoptions_create();

    char* read;
    size_t read_len;
    char* err = NULL;
    read = leveldb_get(db, roptions, key_input, key_input_sz, &read_len, &err);

    if (err != NULL) {
      Nan::ThrowError("Read fail");
      return;
    }

    leveldb_free(err);
    err = NULL;

    leveldb_readoptions_destroy(roptions);

    if (read_len <= 0) {
      info.GetReturnValue().Set(Nan::Undefined());
    } else {
      info.GetReturnValue().Set(Nan::NewBuffer(read, read_len).ToLocalChecked());
    }
  }

  static NAN_METHOD(Put) {
    if (info.Length() != 2) {
      Nan::ThrowTypeError(
          "Incorrect number of arguments; `key` and `value` required");
      return;
    }

    if (!node::Buffer::HasInstance(info[0])) {
      Nan::ThrowTypeError("Incorrect type of `key`, Buffer was expected");
      return;
    }

    if (!node::Buffer::HasInstance(info[1])) {
      Nan::ThrowTypeError("Incorrect type of `value`, Buffer was expected");
      return;
    }

    Local<Object> input = info[0]->ToObject();
    const char* key_input = node::Buffer::Data(input);
    size_t key_input_sz = node::Buffer::Length(input);

    input = info[1]->ToObject();
    const char* value_input = node::Buffer::Data(input);
    size_t value_input_sz = node::Buffer::Length(input);

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());
    leveldb_t* db = instance->db_;

    leveldb_writeoptions_t* woptions;
    woptions = leveldb_writeoptions_create();
    char* err = NULL;
    leveldb_put(db, woptions, key_input, key_input_sz, value_input, value_input_sz, &err);

    if (err != NULL) {
      Nan::ThrowError("Write fail");
      return;
    }
    leveldb_free(err);
    err = NULL;

    leveldb_writeoptions_destroy(woptions);
    info.GetReturnValue().Set(Nan::Undefined());
  }

  static NAN_METHOD(Delete) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Incorrect number of arguments; `key` required");
      return;
    }

    if (!node::Buffer::HasInstance(info[0])) {
      Nan::ThrowTypeError("Incorrect type of `key`, Buffer was expected");
      return;
    }

    Local<Object> input = info[0]->ToObject();
    const char* key_input = node::Buffer::Data(input);
    size_t key_input_sz = node::Buffer::Length(input);

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());
    leveldb_t* db = instance->db_;

    leveldb_writeoptions_t* woptions;
    woptions = leveldb_writeoptions_create();
    char* err = NULL;
    leveldb_delete(db, woptions, key_input, key_input_sz, &err);

    if (err != NULL) {
      Nan::ThrowError("Delete fail");
      return;
    }

    leveldb_free(err);
    err = NULL;

    info.GetReturnValue().Set(Nan::Undefined());
  }

  static NAN_METHOD(IterNew) {
    if (info.Length() != 0) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());

    leveldb_readoptions_t* roptions;
    roptions = leveldb_readoptions_create();

    leveldb_iterator_t* iterator = leveldb_create_iterator(instance->db_, roptions);
    leveldb_iter_seek_to_first(iterator);
    uint32_t id = instance->iterator_count_++;
    instance->iterators_[id] = iterator;
    instance->iterator_options_[id] = roptions;

    info.GetReturnValue().Set(Nan::New((uint32_t) id));
  }

  static NAN_METHOD(IterNext) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());

    uint32_t id = (uint32_t) v8::Number::Cast(*info[0])->Value();
    leveldb_iterator_t* iter = instance->iterators_[id];
    leveldb_iter_next(iter);

    info.GetReturnValue().Set(Nan::New((uint32_t) id));
  }

  static NAN_METHOD(IterValid) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());

    uint32_t id = (uint32_t) v8::Number::Cast(*info[0])->Value();
    uint8_t valid = leveldb_iter_valid(instance->iterators_[id]);

    info.GetReturnValue().Set(Nan::New(valid == 0 ? 0 : (id == 0 ? 1 : id)));
  }

  static NAN_METHOD(IterDestroy) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());

    uint32_t id = (uint32_t) v8::Number::Cast(*info[0])->Value();
    leveldb_iter_destroy(instance->iterators_[id]);
    instance->iterators_.erase(id);
  }

  static NAN_METHOD(IterKey) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());

    uint32_t id = (uint32_t) v8::Number::Cast(*info[0])->Value();
    size_t key_sz;
    const char* key = leveldb_iter_key(instance->iterators_[id], &key_sz);

    info.GetReturnValue().Set(Nan::CopyBuffer(key, key_sz).ToLocalChecked());
  }

  static NAN_METHOD(IterValue) {
    if (info.Length() != 1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    }

    Database* instance = Nan::ObjectWrap::Unwrap<Database>(info.Holder());

    uint32_t id = (uint32_t) v8::Number::Cast(*info[0])->Value();

    size_t value_sz;
    const char* value = leveldb_iter_value(instance->iterators_[id], &value_sz);

    info.GetReturnValue().Set(Nan::CopyBuffer(value, value_sz).ToLocalChecked());
  }

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
};

NODE_MODULE(addon, Database::Init)
} // namespace node_leveldb_mcpe_native

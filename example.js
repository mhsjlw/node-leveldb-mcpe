var leveldb=require('./');

console.log(leveldb.open("test"));
console.log(leveldb.put("key", "value"));
console.log(leveldb.get("key"));
console.log(leveldb.delete("key"));
console.log(leveldb.close());

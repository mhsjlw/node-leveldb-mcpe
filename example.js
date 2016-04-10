var leveldb=require('./');

console.log(leveldb.open("test"));
console.log(leveldb.put("asd", "swag"));
console.log(leveldb.get("asd"));
console.log(leveldb.delete("asd"));
console.log(leveldb.close());

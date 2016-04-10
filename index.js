function generateKey(x,z,type) {
  var buf = new Buffer(9);
  buf.writeUInt32LE(x, 0);
  buf.writeUInt32LE(z, 4);
  buf.writeInt8(type, 8);
  return buf;
}

const addon = require('./build/Release/addon');

console.log(addon.open("db"));
console.log(addon.put("key", 3, "why", 3));
console.log(addon.get("key", 3));
//console.log(addon.delete("key", 3));
console.log(addon.close());

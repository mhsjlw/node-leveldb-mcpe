// function generateKey(x,z,type) {
//   var buf = new Buffer(9);
//   buf.writeUInt32LE(x, 0);
//   buf.writeUInt32LE(z, 4);
//   buf.writeInt8(type, 8);
//   return buf;
// }

var addon = require('bindings')('addon.node');

console.log(addon.open("test"));
//console.log(addon.put("asd", 3, "value", 5));
//console.log(addon.get("asd", 3));
//console.log(addon.delete("asd", 3));
console.log(addon.close());

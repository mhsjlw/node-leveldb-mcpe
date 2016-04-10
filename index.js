var addon = require('bindings')('addon.node');

console.log(addon.open("test"));
console.log(addon.put("asd", "swag"));
console.log(addon.get("asd"));
console.log(addon.delete("asd", 3));
console.log(addon.close());

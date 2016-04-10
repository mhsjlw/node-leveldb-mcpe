var addon = require('bindings')('addon.node');

console.log(addon.open("test"));
console.log(addon.put("asd", "swag"));
console.log(addon.get("asd"));
console.log(addon.delete("asd"));
console.log(addon.close());

var addon = require('bindings')('addon.node');

console.log(addon.open("test"));
console.log(addon.put("asd", 3, "swag", 4));
console.log(addon.get("asd", 3));
console.log(addon.delete("asd", 3));
console.log(addon.close());

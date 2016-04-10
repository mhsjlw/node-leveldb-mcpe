node-leveldb-mcpe
=================

> Note: This is under heavy development, things will probably break so don't use this in production

The Node Binding for LevelDB, updated with ZLib support for MCPE

## Installation
We've made it easy, just:
```
$ git clone --recursive https://github.com/mhsjlw/node-leveldb-mcpe.git
$ npm install
```

## Usage
It's not hard, trust me...

```javascript
console.log(addon.open("db"));
console.log(addon.put("key", "value"));
console.log(addon.get("key"));
console.log(addon.delete("key"));
console.log(addon.close());
```

node-leveldb
============

> Note: This is under heavy development, things will probably break so don't use this in production

The Node Binding for LevelDB, updated with ZLib support for MCPE

## Installation
Alright, firstly, make sure you don't have leveldb installed in your path then

    git clone https://github.com/Mojang/leveldb-mcpe
    cd leveldb-mcpe/
    make
    cp --preserve=links libleveldb.* /usr/local/lib
    cp -r include/leveldb /usr/local/include/
    sudo ldconfig

Now, clone this repo and `node-gyp rebuild` or `npm install`

## Usage
It's not hard, trust me...

```javascript
console.log(addon.open("db"));
console.log(addon.put("key", 3, "value", 5));
console.log(addon.get("key", 3));
console.log(addon.delete("key", 3));
console.log(addon.close());
```

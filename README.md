node-leveldb-mcpe
=================

JavaScript bindings for Mojang's fork of LevelDB

## Installation
Ensure you have Node (>= 8, preferably) and CMake installed, then:
```console
$ git clone https://github.com/mhsjlw/node-leveldb-mcpe.git
$ npm install
```

## Usage
```javascript
const leveldb = require('leveldb-mcpe');

leveldb.open('test');
leveldb.put('key', 'value');
leveldb.get('key'); // => 'value'
leveldb.delete('key');
leveldb.close();
```

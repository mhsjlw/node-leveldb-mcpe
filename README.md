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
const levelup = require('levelup');
const db = levelup(new (require('../'))('test'));

db.put('foo', 'bar', function (err) {
  if (err) throw err;
  db.get('foo', function (err, value) {
    if (err) throw err;
    console.log('Got', value);
  });
});
```

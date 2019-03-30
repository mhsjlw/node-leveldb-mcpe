node-leveldb-mcpe
=================

Node bindings for Mojang's fork of LevelDB

## Installation
Ensure you have Node (>= 8, preferably) and CMake installed, then:
```console
$ git clone https://github.com/mhsjlw/node-leveldb-mcpe.git
$ npm install
```

## Usage
Using callbacks:
```javascript
const levelup = require('levelup');
const db = levelup(new (require('../'))('example.db'));

db.put('foo', 'bar', function (err) {
  if (err) throw err;
  db.get('foo', function (err, value) {
    if (err) throw err;
    console.log(value);
  });
});
```

Or, using `async`/`await` syntax:
```javascript
const levelup = require('levelup')
const db = levelup(new (require('../'))('example.db'))

async function entrypoint () {
  await db.put('foo', 'bar')
  await db.put('quux', 'bazz')
  await db.put('lorem', 'ipsum')

  db.createReadStream()
    .on('data', (data) => {
      console.log(data.key, '=', data.value)
    }).on('error', (err) => {
      console.log(err)
    }).on('close', () => {
      console.log('Iteration complete')
    }).on('end', () => {
      console.log('Stream ended')
    })
}

entrypoint().catch(console.log)
```

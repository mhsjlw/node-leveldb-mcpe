const leveldb = require('../');

leveldb.open('test');
leveldb.put('key', 'value');
console.log(leveldb.get('key')); // => 'value'
leveldb.delete('key');
leveldb.close();

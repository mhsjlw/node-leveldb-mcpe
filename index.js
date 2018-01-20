const { Database } = require('bindings')('node_leveldb_mcpe_native.node');

const util = require('util');
const { AbstractLevelDOWN } = require('abstract-leveldown');

class MinecraftLevelDOWN {
  constructor(location) {
    AbstractLevelDOWN.call(this, location);
  }

  _open(options, callback) {
    this._store = new Database(this.location);
    process.nextTick(function () { callback(null, this) }.bind(this));
  }

  _put(key, value, options, callback) {
    this._store.put(key, value);
    process.nextTick(callback);
  }

  _get(key, options, callback) {
    const value = this._store.get(key);

    if (value === undefined) {
        // 'NotFound' error, consistent with LevelDOWN API
        return process.nextTick(function () { callback(new Error('NotFound')) });
    }

    process.nextTick(function () {
        callback(null, value);
    })
  }

  _del(key, options, callback) {
      this._store.delete(key);
      process.nextTick(callback);
  }

  _close(callback) {
      this._store.close();
      process.nextTick(callback);
  }
}

util.inherits(MinecraftLevelDOWN, AbstractLevelDOWN);

module.exports = MinecraftLevelDOWN;

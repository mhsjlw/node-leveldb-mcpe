const levelup = require('levelup');
const db = levelup(new (require('../'))('test'));

db.put('foo', 'bar', function (err) {
  if (err) throw err;
  db.get('foo', function (err, value) {
    if (err) throw err;
    console.log('Got', value);
  });
});

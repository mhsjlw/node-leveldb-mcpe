/* eslint-env jest */

const levelup = require('levelup')

const db = levelup(new (require('../'))('iterator.db'))

test('can iterate over inserted keys', async () => {
  let results = [ ]

  await db.put('foo', 'bar')
  await db.put('quux', 'bazz')
  await db.put('lorem', 'ipsum')

  // FIXME(keegan), is there a better way [?]
  await (new Promise((resolve) => {
    db.createReadStream()
      .on('data', (data) => {
        results.push(data.key)
      }).on('error', (err) => {
        console.log(err)
      }).on('end', () => {
        resolve()
      })
  }))

  expect(results.length).toEqual(3)
})

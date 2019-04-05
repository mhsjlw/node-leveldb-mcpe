/* eslint-env jest */

const levelup = require('levelup')

const db = levelup(new (require('../'))('iterator.db'))

test('can iterate over inserted keys', async () => {
  let results = [ ]

  await db.put(Buffer.from('foo'), Buffer.from('bar'))
  await db.put(Buffer.from('quux'), Buffer.from('bazz'))
  await db.put(Buffer.from('lorem'), Buffer.from('ipsum'))

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

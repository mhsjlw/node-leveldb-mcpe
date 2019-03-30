/* eslint-env jest */

const levelup = require('levelup')

const db = levelup(new (require('../'))('big.db'))

test('can write 1000 keys', async () => {
  const base = 'key'
  let values = [ ]

  for (let i = 0; i < 1000; i++) {
    values[i] = i.toString()
  }

  let promises = [ ]

  for (let i = 0; i < 1000; i++) {
    promises.push(db.put(base + i, values[i]))
  }

  await Promise.all(promises)
  promises = [ ]

  for (let i = 0; i < 1000; i++) {
    promises.push(db.get(base + i))
  }

  let ret = await Promise.all(promises)

  expect(ret).toEqual(values)
})

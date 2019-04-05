/* eslint-env jest */

const levelup = require('levelup')

const db = levelup(new (require('../'))('buffer.db'))

test('can read and write buffers', async () => {
  const key = Buffer.from('key')
  const value = Buffer.from('value')

  await db.put(key, value)
  const ret = await db.get(key)
  return expect(ret).toEqual(value)
})

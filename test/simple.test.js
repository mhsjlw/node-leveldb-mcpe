/* eslint-env jest */

const levelup = require('levelup')

const db = levelup(new (require('../'))('simple.db'))

test('can open and close database', async () => {
  const local = levelup(new (require('../'))('open_and_close.db'))
  await local.close()
})

test('can read and write string', async () => {
  const key = Buffer.from('key')
  const value = Buffer.from('value')

  await db.put(key, value)
  const ret = await db.get(key)
  return expect(ret).toEqual(value)
})

test('can delete key', async () => {
  const key = Buffer.from('foo')
  const value = Buffer.from('bar')

  await db.put(key, value)
  return expect(db.del(key)).resolves.toEqual(undefined)
})

test('throws error on non-existent key', async () => {
  const key = Buffer.from('foobar')

  return expect(db.get(key)).rejects.toThrow(`Key not found in database [${key}]`)
})

const levelup = require('levelup')
const db = levelup(new (require('../'))('example.db'))

async function entrypoint () {
  await db.put(Buffer.from('foo'), Buffer.from('bar'))
  await db.put(Buffer.from('quux'), Buffer.from('bazz'))
  await db.put(Buffer.from('lorem'), Buffer.from('ipsum'))

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

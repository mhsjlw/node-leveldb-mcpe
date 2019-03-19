const levelup = require('levelup');

const db = levelup(new (require('../'))('buffer.db'));

test('can read and write buffers', async () => {
    const key = (Buffer.from('key', 'ascii')).toString();
    const value = (Buffer.from('value', 'ascii')).toString();

    await db.put(key, value);
    const ret = await db.get(key);
    return expect(ret).toBe(value);
});

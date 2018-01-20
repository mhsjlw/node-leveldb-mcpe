const levelup = require('levelup');

const db = levelup(new (require('../'))('buffer.db'));

test('can read and write buffers', async () => {
    const key = (new Buffer('key')).toString();
    const value = (new Buffer('value')).toString();

    await db.put(key, value);
    const ret = await db.get(key);
    return expect(ret).toBe(value);
});

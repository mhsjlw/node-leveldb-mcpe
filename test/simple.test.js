const levelup = require('levelup');

const db = levelup(new (require('../'))('simple.db'));

test('can open and close database', async () => {
    const local = levelup(new (require('../'))('open_and_close.db'));
    await local.close();
});

test('can read and write string', async () => {
    const key = 'key';
    const value = 'value';

    await db.put(key, value);
    const ret = await db.get(key);
    return expect(ret).toBe(value);
});

test('can delete key', async () => {
    const key = 'foo';
    const value = 'bar';

    await db.put(key, value);
    const ret = await db.get(key);

    return expect(db.del(key)).resolves.toEqual(undefined);
});

test('throws error on non-existent key', async () => {
    const key = 'foobar';

    return expect(db.get(key)).rejects.toThrow(`Key not found in database [${key}]`);
});

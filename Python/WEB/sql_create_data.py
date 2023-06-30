import sqlite3

conn = sqlite3.connect('practice.sqlite')
cur = conn.cursor()

cur.execute('DROP TABLE IF EXISTS Counts')

cur.execute('''
CREATE TABLE Ages (name VARCHAR(128), age INTEGER)''')

cur.execute('INSERT INTO Ages (name, age) VALUES (?, ?)',('Khelis', 21))



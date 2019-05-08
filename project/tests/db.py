import sqlite3
import os

db_path = './test.sqlite3'
try:
	os.remove(db_path)
except:
	pass


db = sqlite3.connect(db_path)

c = db.cursor()

c.execute("CREATE TABLE Data (location TEXT)")

c.execute("ALTER TABLE Data ADD COLUMN 'AA' INTEGER DEFAULT 0")
c.execute("ALTER TABLE Data ADD COLUMN 'AB' INTEGER DEFAULT 0")
c.execute("ALTER TABLE Data ADD COLUMN 'AC' INTEGER DEFAULT 0")
c.execute("ALTER TABLE Data ADD COLUMN 'AD' INTEGER DEFAULT 0")

c.execute("INSERT INTO Data('location', 'AA', 'AB', 'AC', 'AD') VALUES('a', -10, -20, -30, -40)")
c.execute("INSERT INTO Data('location', 'AA', 'AB', 'AC', 'AD') VALUES('a', -11, -21, -31, -41)")
c.execute("INSERT INTO Data('location', 'AA', 'AB', 'AC', 'AD') VALUES('b', -10, -20, -32, -42)")
c.execute("INSERT INTO Data('location', 'AA', 'AB', 'AC', 'AD') VALUES('b', -13, -23, -33, -43)")
c.execute("INSERT INTO Data('location', 'AA', 'AB', 'AC', 'AD') VALUES('c', -14, -24, -34, -44)")

db.commit()



print(list(map(lambda x: x[0], db.execute('select * from Data').description)))


# c.execute("ALTER TABLE Data ADD COLUMN '1:5' INTEGER DEFAULT 0")

c.execute("SELECT * FROM Data")
print(c.fetchall())

c.execute("SELECT DISTINCT (location) FROM Data")
print(c.fetchall())

c.execute("SELECT * FROM Data WHERE AA=-10 AND AB=-20")
print(c.fetchall())

c.execute("SELECT * FROM Data WHERE location='a' AND AA=-10 AND AB=-20")
print(c.fetchall())

c.execute("PRAGMA table_info(Data)")
c.fetchall()

# c.execute('CREATE TABLE Locations (location TEXT UNIQUE)')

# raw_locations = ["Aravinds Room", "Vinays Room", "Krishnas Room", "Aravinds Room"]
# for location in raw_locations:
# 	c.execute('INSERT OR IGNORE INTO Locations(location) VALUES(\'%s\')' % (location))
# db.commit()

# c.execute('SELECT (location) FROM Locations')
# locations = c.fetchall()
# print(locations)

# c.execute('SELECT COUNT(location) FROM Locations')
# n_locations = c.fetchall()
# print(n_locations)


# raw_macID = ['1:1', '1:2', '1:3', '1:4']
# for location in locations:
# 	table_list = ''
# 	for macid in raw_macID:
# 		table_list += "'%s' INTEGER" % macid
# 	c.execute("CREATE TABLE '%s'(%s)" % (location, table_list))
# db.commit()

db.close()


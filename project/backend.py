import sqlite3
from os import path

import config

class Backend(object):
	"""docstring for Backend"""
	def __init__(self):
		super(Backend, self).__init__()

		db = sqlite3.connect(config.path.db)
		# https://stackoverflow.com/questions/7831371/is-there-a-way-to-get-a-list-of-column-names-in-sqlite
		self.stored_macids = list(map(lambda x: x[0], db.execute('select * from Data').description))[1:]
		db.close()

	def find_by_wifi(self, wifiDict):
		db = sqlite3.connect(config.path.db)
		c = self.db.cursor()

		# p_location
		locations = c.execute("SELECT DISTINCT (location) FROM Data")
		p_location = 1.0/len(locations)

		# p_wifi
		recievd_macids = wifiDict.keys()

		for mac_id in recievd_macids:
			if mac_id not in self.stored_macids:
				del recievd_macids[mac_id]

		recievd_macids = wifiDict.keys()

		# macids are numerals. Have to be a+hash() ed for column names
		query_string = 'a%s = %d' % (hash(recievd_macids[0]), wifiDict[recievd_macids[0]])
		for mac_id in recievd_macids[1:]:
			query_string += 'AND a%s = %d' % (hash(mac_id), wifiDict[mac_id])

		sameWiFiRecords = c.execute("SELECT * FROM Data WHERE %s" % query_string)
		p_wifi = 1.0/len(sameWiFiRecords)

		probabilities = []
		for location in locations:
			p_wifi_location = 1.0/c.execute("SELECT * FROM Data WHERE location = %s AND %s" % (location, query_string))
			p_location_wifi = (p_wifi_location*p_wifi)/(p_location)
			probabilities.append(p_location_wifi)

		db.close()

		result = zip(locations, probabilities)

		return max(result, key=lambda item:item[1]) # return location with max probability

	def add_location_data_to_db(self, locationName, wifiDict):
		db = sqlite3.connect(config.path.db)
		c = self.db.cursor()

		recievd_macids = wifiDict.keys()

		for mac_id in recievd_macids:
			if mac_id not in self.stored_macids:
				c.execute("ALTER TABLE Data ADD COLUMN a%s INTEGER DEFAULT 0" % hash(mac_id))

		column_string = ','.join(['a'+hash(i) for i in recievd_macids])
		data_string = ','.join([str(i) for i in wifiDict.values()])

		c.execute('INSERT INTO Data(location,%s) VALUES (%s,%s)' % (column_string, locationName, data_string))
		db.commit()

		db.close()

	def update_location_in_db(self, family, device, location, time):
		db = sqlite3.connect(config.path.db)
		c = self.db.cursor()

		c.execute('INSERT INTO Locations(family,device,location, seenAt) VALUES(%s,%s,%s,%s)' % (family,device,location,time))

		db.commit()
		db.close()

	def get_last_known_location(self, family, device):
		db = sqlite3.connect(config.path.db)
		c = self.db.cursor()

		c.execute('SELECT (location) FROM Locations WHERE family = %s AND device = %s ORDER BY time LIMIT 1' % (family,device,location,time))
		location = c.fetchall()

		db.close()

		return location


		
		
def runml():
	conn = connect('')
from flask import Flask
from flask import request
from flask import jsonify
from flask import render_template

import socket
import time

current_milli_time = lambda: int(round(time.time() * 1000))

class Path:
	db = './databases/db.sqlite3'
class Config:
	path = Path

config = Config

backend = Backend()


app = Flask(__name__)

@app.route('/<family>/<device>')
def main(family, device):

	render_template('track.html', family=family, device=device, location=backed.get_last_known_locations(family, device), ip=socket.gethostbyname(socket.gethostname()))


# @app.route('/api/find/', methods=['GET']) # All families and their devices
# @app.route('/api/find/<family>/', methods=['GET']) # All devices of family
@app.route('/api/find/<family>/<device>/', methods=['GET']) # Only a single device
def api_find(family=None, device=None):
	"""
		This function returns last known location of family/device
		based on the url requested.
	"""

	last_known_location = backed.get_last_known_locations(family, device) # returns json

	return last_known_location


@app.route('/api/track/<family>/<device>/', methods=['POST']) # both family and device are required params
def api_track(family, device):
	"""
		Post wifi MAC and RSSI to this function as JSON and get your location in return.

		{
	
			'family': 'familyName',
			'device': 'deviceName',
			'time': 'time'*,
			'wifi': {
				'mac_1': -10,
				'mac_2': -20,
				...
			}
		}
	"""
	json = request.json
	location = backend.find_by_wifi(json['wifi'])

	backend.update_location_in_db(self, family, device, location, current_milli_time())

	return str(location)


@app.route('/api/learn/<location>/', methods=['POST'])
def api_learn(location):
	"""
	POST
	{   // * means optional
		'family': 'familyname',
		'device': 'devicename',
		'time': 'local ms time from epoch',
		'location': 'locationName',
		'data': {
			['macid': RSSI]
		}
	}
	"""

	json = request.json

	if 'location' not in json:
		return jsonify({'success': False, 'message': 'Location not provided'})

	if 'data' not in json:
		return jsonify({'success': False, 'message': 'Wifi data not provided'})

	backend.add_location_data_to_db(self, location, json['data'])
	backend.update_location_in_db(self, json['family'], json['device'], location, current_milli_time())

	return "added data"

if __name__ == "__main__":
	app.run(host='0.0.0.0', port=8000)
import urllib.error, urllib.request, urllib.parse
import json
target = 'http://py4e-data.dr-chuck.net/json?'

# University of Colorado Boulder
local = input('Enter location : ')
url = target +  urllib.parse.urlencode({'address': local, 'key': 42})
print('retriving', url)
data = urllib.request.urlopen(url).read()
print('retrived', len(data), 'characters')
js = json.loads(data)
print(json.dumps(js, indent = 4))
print('place_id', js['result'][0]['place_id'])
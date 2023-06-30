import urllib.request, urllib.parse, urllib.error
import xml.etree.ElementTree as ET

serviceurl = 'http://maps.googleapis.com/maps/api/geocode/xml?'

# url = input('Enter location: ')
url =  'http://py4e-data.dr-chuck.net/comments_969230.xml'
print('Retrieving', url)
uh = urllib.request.urlopen(url)
data = uh.read()
print('Retrieved', len(data), 'characters')
print(data.decode())
tree = ET.fromstring(data)

counts =  tree.findall('.//count')
print ("Count: " + str(len(counts)))
print (counts)
accumulator = 0

for count in counts:
    accumulator += int(count.text)

print ("Sum:" + str(accumulator))
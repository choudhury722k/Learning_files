import json
import urllib.request, urllib.parse, urllib.error
import xml.etree.ElementTree as ET

# http://py4e-data.dr-chuck.net/comments_969231.json


# url = input('Enter location: ')
address = urllib.request.urlopen('http://py4e-data.dr-chuck.net/comments_969231.json')
data = address.read()

print('Retrieving', 'http://py4e-data.dr-chuck.net/comments_969231.json')
print('Retrieved', len(data), 'characters')

info = json.loads(data)
info = info["comments"]

total = 0

for item in info:
    print(item)
    print("Count: ",item["count"])
    total = total + int(item["count"])
    print("Sum: ", total)

print("Final sum: ", total)


# The JSON format was inspired by the object and array format used in the JavaScript language.
# But since Python was invented before JavaScript, Python’s syntax for dictionaries and lists
# inﬂuenced the syntax of JSON. So the format of JSON is nearly identical to a combination of Python
# lists and dictionaries.

# In general, JSON structures are simpler than XML because JSON has fewer capabilities than XML.
# But JSON has the advantage that it maps directly to some combination of dictionaries and lists.
# And since nearly all programming languages have something equivalent to Python’s dictionaries and lists,
# JSON is a very natural format to have two cooperating programs exchange data.
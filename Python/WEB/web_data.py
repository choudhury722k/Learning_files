import urllib.request, urllib.parse, urllib.error

fhand = urllib.request.urlopen('http://data.pr4e.org/romeo.txt')
counts = dict() 
for line in fhand:
  words = line.decode().split() 
  for word in words: 
    counts[word] = counts.get(word, 0) + 1 
print(counts)

img = urllib.request.urlopen('http://data.pr4e.org/cover3.jpg').read() 
fhand1 = open('cover3.jpg', 'wb') 
fhand1.write(img) 
fhand1.close()
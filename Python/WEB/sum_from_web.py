  
from urllib.request import urlopen
from bs4 import BeautifulSoup
import ssl

# Ignore SSL certificate errors
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

#url = input('Enter - ')
html = urlopen('http://py4e-data.dr-chuck.net/comments_969228.html', context=ctx).read()
soup = BeautifulSoup(html, "html.parser")
print(soup)
spans = soup('span')
numbers = []

for span in spans:
    numbers.append(int(span.string))

print (sum(numbers))
import urllib.request, urllib.parse, urllib.error
from bs4 import BeautifulSoup
import ssl

# Ignore SSL certificate errors
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE
# http://py4e-data.dr-chuck.net/known_by_Fikret.html

# url = input('Enter - ')
url = 'http://py4e-data.dr-chuck.net/known_by_Fikret.html'
html = urllib.request.urlopen(url, context=ctx).read()
soup = BeautifulSoup(html, 'html.parser')

# Retrive the web page script in file format(in short like a text editor)
# print(soup)
# Retrieve all of the anchor tags
tags = soup('a')
# print(tags)
for tag in tags:
    print(tag.get('href', None))

# One of the common uses of the urllib capability in Python is to scrape the web. 
# Web scraping is when we write a program that pretends to be a web browser and retrieves pages, 
# then examines the data in those pages looking for patterns.
# As an example, a search engine such as Google will look at the source of one web page 
# and extract the links to other pages and retrieve those pages, extracting links, and so on. 
# Using this technique, Google spiders its way through nearly all of the pages on the web.
# Google also uses the frequency of links from pages it ﬁnds to a particular 
# page as one measure of how “important” a page is and how high the page should appear in its search results.

# <h1>The First Page</h1>
#  <p> If you like, you can switch to the
#  <a href="http://www.dr-chuck.com/page2.htm">
#  Second Page</a>. </p>

# We can construct a well-formed regular expression to match and extract the link values
# from the above text as follows:
# href="http[s]?://.+?"
#  Our regular expression looks for strings that start with
#  “href="http://” or “href="https://”, followed by one or more characters (.+?),
#  followed by another double quote.
#  The question mark behind the [s]? indicates to search for the string “http” followed by zero or one “s”.
#  The question mark added to the .+? indicates that the match is to be done in a “non-greedy”
#  fashion instead of a “greedy” fashion.
#  A non-greedy match tries to ﬁnd the smallest possible matching string and
#  a greedy match tries to ﬁnd the largest possible matching string.

# Even though HTML looks like XML1 and some pages are carefully constructed to be XML,
# most HTML is generally broken in ways that cause an XML parser to reject the entire
# page of HTML as improperly formed.
# There are a number of Python libraries which can help you parse HTML and extract data from the pages.
# Each of the libraries has its strengths and weaknesses and you can pick one based on your needs.
# As an example, we will simply parse some HTML input and extract links using the BeautifulSoup library.
# BeautifulSoup tolerates highly ﬂawed HTML and still lets you easily extract the data you need


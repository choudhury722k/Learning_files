import socket

mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
mysock.connect(('data.pr4e.org', 80))
cmd = 'GET http://data.pr4e.org/intro-short.txt HTTP/1.0\r\n\r\n'.encode()
mysock.send(cmd)

while True:
    data = mysock.recv(512)
    if len(data) < 1:
        break
    print(data.decode())

mysock.close()


# First the program makes a connection to port 80 on the server www.py4e.com. 
# Since our program is playing the role of the “web browser”, 
# the HTTP protocol says we must send the GET command followed by a blank line. 
# \r\n signiﬁes an EOL (end of line), so \r\n\r\n signiﬁes nothing between two EOL sequences.
# That is the equivalent of a blank line.
# Once we send that blank line, we write a loop that receives data in 512-character 
# chunks from the socket and prints the data out until there is no more data to read 
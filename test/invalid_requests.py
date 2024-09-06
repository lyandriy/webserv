import socket

requests = []

requests.append("GET / HTTP/1.1\r\nHost: localhost:8080\r\n")

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 8080))
for request in requests:
	sock.sendall(request.encode('utf-8'))
	response = sock.recv(4096)
	print(response.decode('utf-8'))
sock.close()
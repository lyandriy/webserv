import socket

requests = []

# requests.append(" GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n") # espacio al principio de la request
# requests.append("GET / HTTP/1.1\r\nHost: localhost:8080\r\n") #falta un \r\n final
# requests.append("AGET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n") # método absurdo
# requests.append("GETA / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n") # método absurdo
requests.append("GET / HTTP1.1\r\nHost: localhost:8080\r\n\r\n") # protocolo sin /
requests.append("GET / HTTP/1.0\r\nHost: localhost:8080\r\n\r\n") # versión protocolo a priori inválida
requests.append("GET / HTTP/2.0\r\nHost: localhost:8080\r\n\r\n") # versión protocolo inválida
requests.append("GET / FTP/1.1\r\nHost: localhost:8080\r\n\r\n") # protocolo inválido
requests.append("GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n") # protocolo inválido


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 8080))
for request in requests:
	sock.sendall(request.encode('utf-8'))
	response = sock.recv(4096)
	print(response.decode('utf-8'))
sock.close()
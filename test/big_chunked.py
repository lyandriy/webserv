import socket
import time

requests: list[str] = []

i = 0
requests.append(																	# Esta es válida, simula una petición hecha con header Transfer-Encoding: chunked
	"GET /index.html HTTP/1.1\r\n"
	"Host: localhost:8080\r\n"
	"Transfer-Encoding: chunked\r\n"
	"Content-Type: text/plain\r\n"
	"Connection: keep-alive\r\n"
	"\r\n"
	"362\r\n"  # 866 bytes 
    + "A" * 866 + "\r\n"  
    "200\r\n"  # 512 bytes 
    + "B" * 512 + "\r\n"
	"0\r\n\r\n"
)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 8080))
for request in requests:
	sock.sendall(request.encode('utf-8'))
	response = sock.recv(4096)
	print(response.decode('utf-8'))
	time.sleep(0.5)	
sock.close()    


# requests.append(f"GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\n\r\n")
# test pendientes:
	# menos de 3 elementos en request line
	# más de 3 elementos en request line
	# request con headers repetidos, misma key pero diferente valor
	# request con parámetros repetidos, misma key pero diferente valor
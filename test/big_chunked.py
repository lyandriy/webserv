import socket
import time

requests: list[str] = []

i = 0
requests.append(																	# Esta es válida, simula una petición hecha con header Transfer-Encoding: chunked
	"GET /index.html HTTP/1.1\r\n"
	"Host: localhost:8081\r\n"
	"Transfer-Encoding: chunked\r\n"
	"Content-Type: text/plain\r\n"
	"Connection: keep-alive\r\n"
	"\r\n"
	"3E8\r\n"  # 1000 bytes 
    + "A" * 1000 + "\r\n"  
    "3E8\r\n"  # 1000 bytes 
    + "C" * 1000 + "\r\n"
	"3E8\r\n"  # 1000 bytes 
    + "D" * 1000 + "\r\n"
	"3E8\r\n"  # 1000 bytes 
    + "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis hendrerit pharetra lorem eu euismod. Aenean quis eros vel lectus dignissim cursus. Proin nec lorem vel eros cursus egestas sed placerat nulla. Aenean pellentesque aliquam sem, at ornare sapien egestas et. Praesent cursus dictum laoreet. Maecenas suscipit nisl ut semper facilisis. Donec volutpat et lorem sed eleifend. Maecenas lacus justo, sodales vel tempor et, fringilla et augue. Integer blandit magna nulla, at luctus risus congue in. Suspendisse congue, risus semper pharetra aliquam, velit dui convallis augue, ut elementum ante ligula ac nunc. Maecenas ac turpis sit amet nisi porta porta sed ut lacus. Maecenas bibendum mollis magna eget finibus.Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Curabitur magna leo, egestas et fermentum eu, imperdiet eget elit. Sed ligula massa, blandit ac bibendum et, bibendum quis augue. Ut scelerisque aliquet neque a fringilla. Quisque porta ante." + "\r\n"
	"0\r\n\r\n"
)

# "200\r\n"  # 512 bytes 
#     + "B" * 512 + "\r\n"
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 8081))
for request in requests:
	sock.sendall(request.encode('utf-8'))
	response = sock.recv(4096)
	print(response.decode('utf-8'))
	time.sleep(0.5)	
sock.close()    
print(len(requests[0]))


# requests.append(f"GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\n\r\n")
# test pendientes:
	# menos de 3 elementos en request line
	# más de 3 elementos en request line
	# request con headers repetidos, misma key pero diferente valor
	# request con parámetros repetidos, misma key pero diferente valor
import socket
import time
import requests as rq

requests: list[str] = []

i = 0
requests.append(f" GET / HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # espacio al principio de la request
i += 1
requests.append(f"GET / HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n") #falta un \r\n final
i += 1
requests.append(f"AGET / HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # método absurdo
i += 1
requests.append(f"GETA / HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # método absurdo
i += 1
requests.append(f"GET / HTTP1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # protocolo sin /
i += 1
requests.append(f"GET / HTTP/1.0\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # versión protocolo a priori inválida
i += 1
requests.append(f"GET / HTTP/2.0\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # versión protocolo inválida
i += 1
requests.append(f"GET / FTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # protocolo inválido
i += 1
requests.append(f"GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # OK
i += 1
requests.append(f"GET / HTTP/1.1\r\nHost: localhost:8o8o\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # puerto inválido
i += 1
requests.append(f"GET / HTTP/1.1\r\nHost: localhost:8080\r\nPeticion:{i}\r\nConnection: keep-alive\r\n\r\n") # OK
i += 1
requests.append(																	# Esta es válida, simula una petición hecha con header Transfer-Encoding: chunked
	"POST /upload HTTP/1.1\r\n"
	"Host: localhost\r\n"
	"Transfer-Encoding: chunked\r\n"
	"Content-Type: text/plain\r\n"
	"\r\n"
	"7\r\nMozilla\r\n"
	"9\r\nDeveloper\r\n"
	"7\r\nNetwork\r\n"
	"0\r\n\r\n"
)
# test pendientes:
	# menos de 3 elementos en request line
	# más de 3 elementos en request line
	# request con headers repetidos, misma key pero diferente valor
	# request con parámetros repetidos, misma key pero diferente valor
url_base = 'http://localhost:8081'

headers = {
	"Host": "localhost:8081",
    "Peticion": "1",
    "Connection": "keep-alive",
}
url = url_base + '/'
response = rq.get(url, headers=headers)
print(response.status_code, response.headers, response.text, response.content)
print("-"*50)
url = url_base
response = rq.get(url, headers=headers)
print(response.status_code, response.headers, response.text)
print("-"*50)
x = input("Enviar siguiente petición?:\n")
if x == 'y' or x == 'yes' or x == 's' or x == 'si':
	continue
else:
exit()

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('http://localhost', 8080))
for request in requests:
	print(request)
	sock.sendall(request.encode('utf-8'))
	response = sock.recv(4096)
	print(response.decode('utf-8'))
	print("\n\n\n")
	x = input("Enviar siguiente petición?:\n")
	if x == 'y' or x == 'yes' or x == 's' or x == 'si':
		continue
	else:
		break 
sock.close()
# for request in requests:
# 	sock.sendall(request.encode('utf-8'))
# 	response = sock.recv(4096)
# 	print(response.decode('utf-8'))
# 	time.sleep(0.5)	
# sock.close()    

import requests
from colorama import Fore, Style

def another_request():
	x = input("Enviar siguiente petición?:\n")
	if not (x == 'y' or x == 'yes' or x == 's' or x == 'si' or len(x) == 0):
		exit()

press_to_continue = "Presiona una tecla para continuar.\n"
# 0 pruebas para la configuración correcta completa: (config/conf)
input("Esta es una request sencilla a la página principal por defecto \
	  del servidor\n" + press_to_continue)

url_base = 'http://localhost:8081'
headers = {
    "Connection": "keep-alive",
}
test = 0

# 1 petición a ruta por defecto (index)
test += 1
url = url_base + ''
response = requests.get(url, headers=headers)
print("URL de la petición: ", url)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
file = open("html/files/index.html", 'r')
content = file.read()
file.close()
if response.text == content:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)
with open("test/outputs/primera_basica.html", 'w+') as f:
	f.write(response.text)
print("\n" + "-"*50 + "\n")

# another_request()


# 2 peticion sencilla de archivo normal
test += 1
url = url_base + '/prueba.html'
response = requests.get(url, headers=headers)
print("URL de la petición: ", url)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
file = open("html/files/prueba.html", 'r')
content = file.read()
file.close()
if response.text == content:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)
print("\n" + "-"*50 + "\n")

# another_request()


# 3 respuesta muy grande
test += 1
url = url_base + "/el_quijote.txt"
response = requests.get(url, headers=headers)
print("URL de la petición: ", url)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
file = open("html/files/el_quijote.txt", 'r', encoding='utf-8')
content = file.read()
file.close()
if response.text[:1000] == content[:1000]:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)

# another_request()


# 4 petición errónea
test += 1
url = url_base + "/esto_no_existe.html"
response = requests.get(url, headers=headers)
print("URL de la petición: ", url)
print("Texto de la respuesta:\n", response.text)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
# esta ruta viene del archivo de configuración
# 	error_page 404 html/files/index.html;
file = open("html/files/index.html", 'r', encoding='utf-8') 
content = file.read()
file.close()
if response.text == content:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)

# another_request()


# 5 autoindex correcto
test += 1
url = url_base + "/python"
response = requests.get(url, headers=headers)
print("URL de la petición: ", url)
# print("Texto de la respuesta:\n", response.text)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
# si exista esta etiqueta de html es que se ha generado una lista
if response.text.find("<li><a href=") != -1: 
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)

# another_request()

# 6 autoindex incorrecto
test += 1
url = url_base + "/directorio_inexistente"
print("URL de la petición: ", url)
response = requests.get(url, headers=headers)
print("Texto de la respuesta:\n", response.text)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
file = open("serverHTML/error/404.html", 'r')
content = file.read()
file.close()
if response.text == content:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)

# another_request()


# 7 servername incorrecto
test += 1
url = 'http://wololo:8081'
try:
	print("URL de la petición: ", url)
	response = requests.get(url, headers=headers)
	print("Texto de la respuesta:\n", response.text)
	print("Código de respuesta:\n", response.status_code)
	print("Cabeceras de la respuesta:\n", response.headers)
except requests.exceptions.ConnectionError:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
finally:
	print(Style.RESET_ALL)

# another_request()

# 8 servername incorrecto en header
test += 1
url = url_base + "/"
headers['Host'] = 'wololo:8081'
print(headers)
print("URL de la petición: ", url)
response = requests.get(url, headers=headers)
print("Texto de la respuesta:\n", response.text)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
file = open("serverHTML/error/400.html", 'r')
content = file.read()
file.close()
if response.text == content:
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)


another_request()

# 9 servername incorrecto en header
test += 1
url = url_base + "/hola"
headers['Host'] = 'localhost:8081'
response = requests.get(url, headers=headers)
print("Texto de la respuesta:\n", response.text)
print("Código de respuesta:\n", response.status_code)
print("Cabeceras de la respuesta:\n", response.headers)
file = open("/test/redirect/prueba.html", 'r')
content = file.read()
file.close()
if response.text == content: # añadir comprobación de response.status_code
	print(Fore.GREEN + f"TEST {test} CORRECTO")
else:
	print(Fore.RED + f"TEST {test} INCORRECTO")
print(Style.RESET_ALL)


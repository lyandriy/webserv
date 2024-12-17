import requests
from colorama import Fore, Style
import os
import time

def another_request():
	x = input("Enviar siguiente petición?:\n")
	if not (x == 'y' or x == 'yes' or x == 's' or x == 'si' or len(x) == 0):
		exit()

# press_to_continue = "Presiona una tecla para continuar.\n"
# # 0 pruebas para la configuración correcta completa: (config/conf)
# input("Esta es una request sencilla a la página principal por defecto \
# 	  del servidor\n" + press_to_continue)

url_base = 'http://localhost:8081'
headers = {
    "Connection": "keep-alive",
    # "Connection": "close",
}
test = 0

def test1():
# 1 petición a ruta por defecto (index)
	global test
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
	print("\n" + "-"*50 + "\n")

	# another_request()

def test2():
# 2 peticion sencilla de archivo normal
	global test
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

def test3():
	# 3 respuesta muy grande
	global test
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

def test4():
	# 4 petición errónea
	global test
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

def test5():
	# 5 autoindex correcto
	global test
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

def test6():
	# 6 autoindex incorrecto
	global test
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

def test7():
	# 7 servername incorrecto
	global test
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

def test8():
	# 8 servername incorrecto en header
	global test
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

def test9():
	# 9 usando location con redirección
	global test
	test += 1
	url = url_base + "/hola"
	headers['Host'] = 'localhost:8081'
	response = requests.get(url, headers=headers)
	print("Texto de la respuesta:\n", response.text)
	print("Código de respuesta:\n", response.status_code)
	print("Cabeceras de la respuesta:\n", response.headers)
	file = open("html/test/redirect/prueba.html", 'r')
	content = file.read()
	file.close()
	if response.text == content: # and response.status_code in (range(300, 400))# añadir comprobación de response.status_code
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	else:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	print(Style.RESET_ALL)
	another_request()

def test10():
	# 10 petición GET con body
	global test
	test += 1
	url = url_base + "/"
	headers['Host'] = 'localhost:8081'
	headers['Content-Type'] = 'text/plain'
	body_content = "Este es un pqueño body para el test"
	response = requests.get(url, headers=headers, data=body_content)
	file = open("html/files/index.html", 'r')
	content = file.read()
	file.close()
	if response.text == content:
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	else:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	print(Style.RESET_ALL)
	print("\n" + "-"*50 + "\n")

	# another_request()

def test11():
	# 11 petición POST básica crea archivo wololo.txt
	try:
		os.remove("html/files/wololo.txt")
	except FileNotFoundError:
		pass
	global test
	test += 1
	url = url_base + "/wololo.txt"
	headers['Host'] = 'localhost:8081'
	headers['Content-Type'] = 'text/plain'
	body_content = "Este es un simple body para el test\n"
	response = requests.post(url, headers=headers, data=body_content)
	file = open("serverHTML/postResponse.html", 'r')
	content = file.read()
	file.close()
	try:
		file2 = open("html/files/wololo.txt", 'r')
		content2 = file2.read()
		file2.close()
	except FileExistsError:
		print(Fore.RED + f"TEST {test} INCORRECTO")
		return	
	if response.text == content and body_content == content2:
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	else:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	print(Style.RESET_ALL)
	print("\n" + "-"*50 + "\n")

	input("""El archivo wololo.txt ha sido creado, si no, 
	   \rel siguiente test se rompe, puedes comprobarlo ahora 
	   \ro pulsar cualquier tecla para continuar""")
	test12()

def test12():
	# 12 petición POST básica añade informacion al archivo wololo.txt
	global test
	test += 1
	try:
		file2 = open("html/files/wololo.txt", 'r')
		wololo_antes = file2.read()
		file2.close()
	except FileExistsError:
		print(Fore.RED + f"TEST {test} INCORRECTO")
		return
	url = url_base + "/wololo.txt"
	headers['Host'] = 'localhost:8081'
	headers['Content-Type'] = 'text/plain'
	body_content = "Este es un otro body para el test 12\n"
	response = requests.post(url, headers=headers, data=body_content)
	file = open("serverHTML/postResponse.html", 'r')
	content = file.read()
	file.close()
	try:
		file2 = open("html/files/wololo.txt", 'r')
		wololo_despues = file2.read()
		file2.close()
		os.remove("html/files/wololo.txt")
	except FileExistsError:
		print(Fore.RED + f"TEST {test} INCORRECTO")
		return
	if response.text == content and wololo_antes + body_content == wololo_despues:
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	else:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	print(Style.RESET_ALL)
	print("\n" + "-"*50 + "\n")
	# another_request()

def test13():
	# 13 petición POST con un body grande no procesable de una vez
	global test
	test += 1

	try:
		file = open('html/test/un_archivo_cualquiera.txt', 'r')
		body_content = file.read()
		file.close()
	except FileNotFoundError:
		return
	url = url_base + "/lorem_ipsum.txt"
	headers['Host'] = 'localhost:8081'
	headers['Content-Type'] = 'text/plain'
	headers['Content-Length'] = str(len(body_content))
	response = requests.post(url, headers=headers, data=body_content)
	print("Longitud del body: " + str(len(body_content)))
	try:
		lorem = open("html/files/lorem_ipsum.txt", 'r')
		lorem_content = lorem.read()
		lorem.close()
	except FileNotFoundError:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	file = open("serverHTML/postResponse.html", 'r')
	content = file.read()
	file.close()
	if response.text == content and lorem_content == body_content:
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	else:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	try:
		os.remove("html/files/lorem_ipsum.txt")
	except FileNotFoundError:
		print("No hay archivo")
	print(Style.RESET_ALL)
	print("\n" + "-"*50 + "\n")
	# another_request()

def test14():
	# 14 request con chunks
	global test
	test += 1
	from chunked import chunked_test
	response = chunked_test()
	try:
		file = open("html/files/indexxx.html")
		file_content = file.read()
		file.close()
	except:
		print(Fore.RED + f"TEST {test} INCORRECTO")
	else:
		if file_content == "MozillaFirefox\nDeveloper\nNetwork":
			print(Fore.GREEN + f"TEST {test} CORRECTO")
	print(Style.RESET_ALL)
			
	""" url = url_base + "/indexxxxxx.html"
	headers = {
		"Host": "localhost:8081",
		"Transfer-Encoding": "chunked",
		"Content-Type": "text/plain"
	}

	def chunked_body():
		chunks = [
			"E\r\nMozillaFirefox\r\n",
			"9\r\nDeveloper\r\n",
			"7\r\nNetwork\r\n",
			"0\r\n\r\n"		
		]
		for chunk in chunks:
			yield bytes(chunk, 'utf-8 ')
	response = requests.post(url, headers=headers, data=chunked_body())

	print("Código de respuesta:", response.status_code)
	print("Texto de la respuesta:", response.text) """

def test15():
	# 15 delete
	global test
	test += 1
	url = url_base + "/un_archivo_para_delete.txt"
	if not os.path.exists("html/files/un_archivo_para_delete.txt"):
		body_content = "Algo de contenido para el archivo del test 15"
		headers['Host'] = 'localhost:8081'
		headers['Content-Type'] = 'text/plain'
		headers['Content-Length'] = str(len(body_content))
		requests.post(url, headers=headers, data=body_content)
	response = requests.delete(url, headers=headers)
	print(response.status_code, response.text)

def test16():
	# 16 cgi basico
	global test
	test += 1
	url = url_base + "/python/hola.py"
	headers['Content-Type'] = 'text/plain'
	headers['Host'] = 'localhost:8081'
	response = requests.get(url, headers=headers)
	with open("serverHTML/error/403.html", 'r') as file:
		content_error = file.read()
	if response.status_code == 403 and response.text == content_error:
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	else:
		print(Fore.GREEN + f"TEST {test} CORRECTO")
	print(Style.RESET_ALL)

def test17():
	# 17 cgi con viariables de entorno
	global test
	test += 1
	url = url_base + "/python/saludo_user_simple.py"
	headers['Content-Type'] = 'text/plain'
	headers['Host'] = 'localhost:8081'
	headers['user'] = "josgarci"
	response = requests.post(url, headers=headers)
	print(Fore.GREEN + response.text)
	print(Style.RESET_ALL)

def test18():
	# 18 cgi duradero pero sin timeout
	global test
	test += 1
	pass

def test19():
	# 19 cgi que llega a timeout
	global test
	test += 1
	pass

def test20():
	# 20 cgi que da error
	global test
	test += 1
	pass

def test21():
	# 21 cgi con post
	global test
	test += 1
	pass

def test22():
	# 22 metodos no aceptados
	global test
	test += 1
	url = url_base + "/"
	headers['Host'] = 'localhost:8081'
	response_get = requests.get(url, headers=headers)
	print("GET", response_get.status_code, response_get.text)
	url = url_base + "/wollllooooolllllooooo"
	body = "Un body para las prueba"
	headers['Content-Type'] = 'text/plain'
	headers['Content-Length'] = str(len(body))
	print(headers)
	response_post = requests.post(url, headers=headers, data=body)
	print("POST", str(response_post.status_code), response_post.text)
	input("COmprueba que existe el archivo wollllooooolllllooooo\n")
	url = url_base + "/test/wollllooooolllllooooo"
	headers.clear()
	response_del = requests.delete(url, headers=headers)
	print("DELETE", str(response_del.status_code), response_del.text)


# probar metodos no aceptados
# probar borrar archivos sin permiso
# hacer test para un get de post previo y recuperar archivo

if __name__ == '__main__':
	# test1()
	# test2()
	# test3()
	# test4()
	# test5()
	# test6()
	# test7()
	# test8()
	# test9()
	# test10()
	# test11()
	# #test12() # el test 12 es llamado dentro del test 11
	# test13()
	# test14()
	# test15()
	# test16()
	# test17()
	# test18()
	# test19()
	# test20()
	# test21()
	test22()


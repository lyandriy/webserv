import requests

try:
    response = requests.get('http://localhost:8081', timeout=5)  # 5 segundos de timeout
    print("Response:", response.status_code)
except requests.exceptions.Timeout:
    print("La solicitud expiró")
except requests.exceptions.RequestException as e:
    print("Error:", e)

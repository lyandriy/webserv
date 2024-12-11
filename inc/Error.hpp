# pragma once

# define OK 200
# define BAD_REQUEST 400 //REQUEST--
# define FORBIDEN 403 //RESPONSE
# define NOT_FOUND 404 //RESPONSE
# define METHOD_NOT_ALLOWED 405 //REQUEST--
# define REQUEST_TIMEOUT 408 //RESPONSE
# define CONTENT_TOO_LARGE 413 //REQUEST--
# define URI_TOO_LONG 414 //REQUEST--
# define INTERNAL_SERVER_ERROR 500 //RESPONSE
# define SERVICE_UNAVAIBLE 503 //RESPONSE
# define HTTP_VERSION_NOT_SUPPORTED 505 //REQUEST--
# define GATEWAY_TIMEOUT 504 //RESPONSE--

# define ROOT_BAD_REQUEST "serverHTML/error/400.html"
# define ROOT_FORBIDEN "serverHTML/error/403.html"
# define ROOT_NOT_FOUND "serverHTML/error/404.html"
# define ROOT_METHOD_NOT_ALLOWED "serverHTML/error/405.html"
# define ROOT_REQUEST_TIMEOUT "serverHTML/error/408.html"
# define ROOT_CONTENT_TOO_LARGE "serverHTML/error/413.html"
# define ROOT_URI_TOO_LONG "serverHTML/error/414.html"
# define ROOT_INTERNAL_SERVER_ERROR "serverHTML/error/500.html"
# define ROOT_SERVICE_UNAVAIBLE "serverHTML/error/503.html"
# define ROOT_HTTP_VERSION_NOT_SUPPORTED "serverHTML/error/505.html"
# define ROOT_HTTP_GATEWAY_TIMEOUT "serverHTML/error/504.html"

# define HTML_AUTOINDEX_BEGIN "<!DOCTYPE html>\n<head>\n    <meta charset=\"UTF-8\">\n    <title>Índice del Directorio</title></head>\n<body>\n    <h1>Índice del Directorio</h1>\n    <ul>\n"
# define HTML_AUTOINDEX_END "    </ul>\n</body>\n</html>\r\n\r\n"
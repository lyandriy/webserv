# pragma once

# define OK 200
# define BAD_REQUEST 400 //REQUEST--
# define FORBIDEN 403 //RESPONSE
# define NOT_FOUND 404 //RESPONSE
# define METHOD_NOT_ALLOWED 405 //REQUEST--
# define REQUEST_TIMEOUT 408 //Response
# define CONTENT_TOO_LARGE 413 //REQUEST--
# define URI_TOO_LONG 414 //REQUEST--
# define INTERNAL_SERVER_ERROR 500 //RESPONSE
# define SERVICE_UNAVAIBLE 503 //response
# define HTTP_VERSION_NOT_SUPPORTED 505 //REQUEST--

# define ROOT_BAD_REQUEST "/home/lyandriy/webserv/html/error/400.html"
# define ROOT_FORBIDEN "./htlm/error/403.html"
# define ROOT_NOT_FOUND "../../htlm/error/404.html"
# define ROOT_METHOD_NOT_ALLOWED "./htlm/error/405.html"
# define ROOT_REQUEST_TIMEOUT "./htlm/error/408.html"
# define ROOT_CONTENT_TOO_LARGE "./htlm/error/413.html"
# define ROOT_URI_TOO_LONG "./htlm/error/414.html"
# define ROOT_INTERNAL_SERVER_ERROR "./htlm/error/500.html"
# define ROOT_SERVICE_UNAVAIBLE "./htlm/error/503.html"
# define ROOT_HTTP_VERSION_NOT_SUPPORTED "./htlm/error/505.html"

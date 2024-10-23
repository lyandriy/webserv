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

# define ROOT_BAD_REQUEST "html/error/400.html"
# define ROOT_FORBIDEN "html/error/403.html"
# define ROOT_NOT_FOUND "html/error/404.html"
# define ROOT_METHOD_NOT_ALLOWED "html/error/405.html"
# define ROOT_REQUEST_TIMEOUT "html/error/408.html"
# define ROOT_CONTENT_TOO_LARGE "html/error/413.html"
# define ROOT_URI_TOO_LONG "html/error/414.html"
# define ROOT_INTERNAL_SERVER_ERROR "html/error/500.html"
# define ROOT_SERVICE_UNAVAIBLE "html/error/503.html"
# define ROOT_HTTP_VERSION_NOT_SUPPORTED "html/error/505.html"
# define AUTOINDEX_FILE "html/autoindex.html"

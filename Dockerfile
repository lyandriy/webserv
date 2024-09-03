#Dockerfile NGINX
FROM debian:buster

RUN apt update
RUN apt-get install nginx -y
RUN apt install openssl -y
RUN apt-get clean

COPY ./default /etc/nginx/sites-available/
COPY ./default /var/www/html/
EXPOSE 8080

CMD ["nginx", "-g", "daemon off;"]
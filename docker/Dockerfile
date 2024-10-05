#Dockerfile NGINX
FROM debian:buster
RUN apt update
RUN apt-get install nginx -y
RUN apt install openssl -y
RUN apt-get clean
COPY ./default /etc/nginx/sites-available/
COPY ./default /var/www/html/
COPY ./index.html /var/www/html/
RUN chown -R www-data:www-data /var/www/html
RUN chmod -R 755 /var/www/html
EXPOSE 8080
CMD ["nginx", "-g", "daemon off;"]
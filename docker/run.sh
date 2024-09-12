docker stop  $(docker ps -aq)
docker rm -f $(docker ps -aq)
docker rmi -f $(docker images -aq)
docker build -t nginx .
docker run -d --name nginx  -p 8080:8080 nginx
docker logs nginx
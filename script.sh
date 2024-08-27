docker stop -f $(docker ps -aq)
docker rm -f $(docker ps -aq)
docker rmi -f $(docker images -aq)
docker build -t nginx .
docker run -d --name nginx nginx
docker logs nginx
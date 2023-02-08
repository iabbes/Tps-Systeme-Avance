# TP9

## Exercice 1

- Récupération de l'image :

docker pull networkboot/dhcpd

- Création du fichier de configuration pour le service :

mkdir data
cd /data
sudo vi dhcpd.conf

- Démarrage du conteneur avec les paramètres appropriés :

docker run --name dhcpd-container --network=host --mount type=bind,src=/data/dhcpd.conf,dst=/data/dhcpd.conf -p 67:67/udp networkboot/dhcpd

- Vérification de la disponibilité du service :

ip addr show

- Arrêt du conteneur associé :

docker stop dhcpd-container

- Sollicitation du service DHCP :

dhclient -4 eth0

- Ajouter une adresse IP à une interface existante :

ip addr add 92.184.124.84/24 dev eth0
ifconfig eth0:0 92.184.124.84 netmask 255.255.255.0 up


## Exercice 2

- Récupération des images :

docker pull mysql:latest
docker pull wordpress:latest

- Création du fichier de configuration docker-compose.yml

vi docker-compose.yml

- Démarrage de la stack :

docker-compose up -d

- Vérification de l'état du service :

docker-compose ps

- Vérifier l'accessibilité de l'application :

http://localhost

- Arrêt du conteneur associé :

docker-compose down

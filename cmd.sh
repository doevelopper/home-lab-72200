docker compose up -d --force-recreate --remove-orphans
docker compose down  --remove-orphans



# Generate a private key
openssl genrsa -out ${1}.key 2048

# Create temp file for config
cat > temp.cnf <<EOF
[ req ]
distinguished_name = req_distinguished_name
req_extensions = v3_req
[ req_distinguished_name ]
[ v3_req ]
subjectAltName = @alt_names
[ alt_names ]
IP.1 = 192.168.60.103
IP.2 = 127.0.0.1
EOF

# Create a csr
openssl req -new -key ${1}.key -out ${1}.csr \
  -subj "/C=FR/ST=Sarthe/L=SSS/O=ACME/OU=ENgineering/CN=${1}" \
  -config temp.cnf

# Sign the cert
openssl x509 -req -in ${1}.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
  -out ${1}.crt -days 7300 -sha256 -extensions v3_req -extfile temp.cnf

# Verify the cert and be sure you see Subject Name Alternative

openssl x509 -in ${1}.crt -text -noout | grep -A1 "Subject Alternative Name"

# Remove temp file

rm temp.cnf



# #!/bin/bash

# # Variables to be set for certificate generation:
# country=""
# state=""
# city=""
# organisation=""
# department=""
# servername=""

# # Change current filepath to scripts directory
# cd "$(dirname "$0")"

# # Generate new folders (that can't get stored in git)
# mkdir -p ollama
# mkdir -p open-webui
# mkdir -p nginx-gateway/cert

# echo "Do you want to generate a certificate? (y/n)"
# read generate_cert
# if [ "$generate_cert" = "y" ]; then
#     # Check if all required variables are set
#     if [ -z "$country" ] || [ -z "$state" ] || [ -z "$city" ] || [ -z "$organisation" ] || [ -z "$department" ] || [ -z "$servername" ]; then
#         echo "Please set the variables at the top of the setup script before proceeding. Script exiting."
#         exit 1
#     else
# 	# Generate SSL-Key and Certificate
#         openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout nginx-gateway/cert/nginx.key -out nginx-gateway/cert/nginx.crt -subj "/C=$country/ST=$state/L=$city/O=$organisation/OU=$department/CN=$servername"
#     fi
# fi

# # Generate containers according to docker-compose.yaml file (must be in the same folder)
# docker compose up -d

# mkdir -p deployment/certs
# openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout deployment/certs/local-dev.key -out deployment/certs/local-dev.crt -subj "/CN=localhost"
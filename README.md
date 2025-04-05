# Project Name

**Description:**  
This project demonstrates how to leverage Docker images for **NGINX** and **Traefik** in the same infrastructure. NGINX serves as a high-performance static content server and reverse proxy, while Traefik provides dynamic service discovery, automatic HTTPS termination, and seamless integration with container orchestration tools like Docker.

---

## Table of Contents

1. [Overview](#overview)  
2. [Prerequisites](#prerequisites)  
3. [Installation](#installation)  
4. [Configuration](#configuration)  
   - [NGINX](#nginx)  
   - [Traefik](#traefik)  
5. [Usage](#usage)  
6. [Contributing](#contributing)  
7. [License](#license)  

---

## Overview

This project sets up a containerized infrastructure using **Docker Compose** to deploy:
- **NGINX**: For serving static content and handling reverse proxying.
- **Traefik**: For dynamic routing, load balancing, and automatic HTTPS termination.

The combination of these two tools allows you to build a robust, scalable, and secure infrastructure suitable for modern web applications.

---
project-root/
├── docker-compose.yml
├── nginx/
│   ├── conf.d/
│   │   └── default.conf
│   └── certs/
│       ├── localhost.crt
│       └── localhost.key
├── static-content/
│   └── index.html
└── traefik/
    └── certs/
        ├── localhost.crt
        └── localhost.key
---

## Prerequisites

Before you begin, ensure you have the following installed on your system:

- **Docker**: [Install Docker](https://docs.docker.com/get-docker/)  
- **Docker Compose**: [Install Docker Compose](https://docs.docker.com/compose/install/)  
- **Domain Name (Optional)**: Required for automatic HTTPS with Traefik.  
- **Basic Knowledge of Docker and Networking**

---

## Installation

1. **Clone the Repository**:
   ```bash
    git clone https://github.com/your-username/your-repo-name.git
    cd your-repo-name
   ```
2. **Set Up Environment Variables:**
    - Copy the .env.example file to .env:
    ```bash    
        cp .env.example .env
    ```
    - Update the .env file with your domain name, email (for Let's Encrypt), and other required configurations.

3. **Start the Services:**

Use Docker Compose to start the services:
    ```bash
    docker-compose up -d
    ``` 
3. **Verify the Deployment:**

    - Check the status of the containers:

    ```bash
    docker ps
    ``` 
    - Access the Traefik dashboard at http://<your-domain>/dashboard.

## Configuration

### NGINX

    NGINX is configured to serve static content and act as a reverse proxy. Its configuration is defined in the nginx/nginx.conf file.

    1. Key Features:
        - Serves static files from the ./static directory.
        - Acts as a reverse proxy for backend services.
    1. Customization:
        - Modify the nginx/nginx.conf file to adjust routing rules or add custom middleware.
        - Place your static files in the ./static directory.
### Traefik

Traefik is configured to dynamically discover services and handle HTTPS termination. Its configuration is defined in the traefik/traefik.yml file.

    Key Features:
        - Automatic HTTPS with Let's Encrypt.
        - Dynamic service discovery via Docker labels.
        - Built-in dashboard for monitoring.
    Customization:
        - Update the traefik/traefik.yml file to modify entry points, middlewares, or providers.
        - Add custom labels to your Docker services for dynamic routing.
## Usage
    1.  Access the Traefik Dashboard:
    Open your browser and navigate to:
    ```txt
    http://<your-domain>/dashboard
    ```  
    2. Serve Static Content with NGINX:
    Place your static files in the ./static directory and access them at:
    ```txt
    http://<your-domain>/static
    ```
    3. Deploy New Services:
    Add new services to your `docker-compose.yml `file and include Traefik labels for automatic routing. Example:
    ```yml
        labels:
        - "traefik.enable=true"
        - "traefik.http.routers.my-service.rule=Host(`my-service.<your-domain>`)"
        - "traefik.http.services.my-service.loadbalancer.server.port=8080"
    ```
    4. Reload Services:
    After making changes to the docker-compose.yml file, reload the services:
    ```bash
        docker-compose up -d
    ```

## Contributing
We welcome contributions! If you'd like to contribute to this project, please follow these steps:

    1. Fork the repository.
    1. Create a new branch (git checkout -b feature/your-feature).
    1. Commit your changes (git commit -m "Add your feature").
    1. Push to the branch (git push origin feature/your-feature).
    1. Open a pull request.


## Accessing Services
After making these changes, here's how you can access the services:

    Service	URL
    Traefik Dashboard	http://localhost:8080
    Traefik Gateway	http://localhost or https://localhost
    Nginx (Static)	http://localhost:8081 or https://localhost:8443
    Who Am I	https://whoami.local
    Static Content	https://static.local
    Grafana	http://localhost:3001

## Additional Notes
###    Hostnames for Services:
    For services like whoami.local and static.local, ensure you map these hostnames to 127.0.0.1 in your /etc/hosts file:

    127.0.0.1 whoami.local
    127.0.0.1 static.local

### Future Services:
    When adding new services, always check for port conflicts and assign unique ports.

### Testing:
    Run docker-compose up -d --force-recreate --remove-orphans to start the services.
    Use docker-compose ps to verify all containers are running without errors.

## License
This project is licensed under the MIT License. Feel free to use, modify, and distribute it as needed.

Additional Resources
NGINX Documentation
Traefik Documentation
Docker Compose Documentation


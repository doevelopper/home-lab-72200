Below is a step-by-step guide to designing a home lab infrastructure using the provided Docker Compose file. This setup is entirely local, running on your own machine, and uses Docker Compose to manage a reverse proxy (Traefik), a sample microservice (Whoami), an optional static content service, and an additional Nginx instance. The goal is to create a functional lab environment with service isolation and HTTPS support using local SSL certificates.

---

### **Step 1: Understand the Docker Compose Structure**

The Docker Compose file defines:

- **Networks**:
  - `frontend`: A bridge network that is not internal, meaning it can communicate with the host (public-facing).
  - `backend`: An internal bridge network, restricting communication to within the Docker environment.

- **Services**:
  - `nginx`: A public-facing Nginx reverse proxy on ports 8081 (HTTP) and 8443 (HTTPS).
  - `traefik`: An internal gateway and reverse proxy on ports 80 (HTTP), 443 (HTTPS), and 8080 (dashboard).
  - `whoami`: A sample microservice accessible via `https://whoami.local` through Traefik.
  - `static-content`: An optional Nginx service serving static files at `https://static.local` through Traefik.

All services use `restart: unless-stopped` to ensure they restart automatically unless manually stopped.

---

### **Step 2: Set Up the Directory Structure**

Create the following directories and files on your local machine to support the volumes defined in the Docker Compose file:

- `./nginx/conf.d`: For Nginx configuration files (read-only).
- `./static-content`: For static content files (e.g., `index.html`) to be served by Nginx instances (read-only).
- `./nginx/certs`: For Nginx's local SSL certificates (read-only).
- `./traefik/certs`: For Traefik's local SSL certificates (read-only).
- `./traefik/dynamic-config.yml`: For Traefik's dynamic configuration (read-only).
- `./traefik/letsencrypt`: For Let's Encrypt storage (not used in this local setup but included for completeness).

Run these commands in your terminal from the directory containing your `docker-compose.yml`:

```bash
mkdir -p nginx/conf.d static-content nginx/certs traefik/certs traefik/letsencrypt
touch traefik/dynamic-config.yml
```

---

### **Step 3: Generate Local SSL Certificates**

Since this is a local setup using HTTPS, you need self-signed certificates for the domains `whoami.local` and `static.local` (and optionally for Nginx). Use OpenSSL to generate them:

#### **For Traefik (`whoami.local` and `static.local`)**:
1. Generate a certificate for `whoami.local`:
   ```bash
   openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
     -keyout traefik/certs/whoami.local.key \
     -out traefik/certs/whoami.local.crt \
     -subj "/CN=whoami.local"
   ```

2. Generate a certificate for `static.local`:
   ```bash
   openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
     -keyout traefik/certs/static.local.key \
     -out traefik/certs/static.local.crt \
     -subj "/CN=static.local"
   ```

#### **For Nginx (Optional)**:
If you plan to configure Nginx to serve content directly, generate a certificate for `localhost`:
```bash
openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
  -keyout nginx/certs/localhost.key \
  -out nginx/certs/localhost.crt \
  -subj "/CN=localhost"
```

These certificates will be mounted into the containers via the volumes specified in the Docker Compose file.

---

### **Step 4: Configure Traefik's Dynamic Configuration**

Traefik uses a dynamic configuration file (`dynamic-config.yml`) to manage certificates and other settings beyond the static configuration provided in the `command` section. Since this is a local setup with static certificates, define the certificates directly instead of using a certificate resolver like Let's Encrypt.

Edit `./traefik/dynamic-config.yml` with the following content:

```yaml
tls:
  certificates:
    - certFile: /certs/whoami.local.crt
      keyFile: /certs/whoami.local.key
    - certFile: /certs/static.local.crt
      keyFile: /certs/static.local.key
```

**Note**: The original Docker Compose file includes `traefik.http.routers.whoami.tls.certresolver=localcert` in the labels for `whoami` and `static-content`. However, since we're using static certificates and not an ACME resolver (like Let's Encrypt), this label is unnecessary and may cause errors if `localcert` isn't defined. You can either:
- Remove the `traefik.http.routers.<service>.tls.certresolver=localcert` labels from the `whoami` and `static-content` services in the Docker Compose file, or
- Leave them as-is and assume Traefik will fallback to the certificates defined above (though removing them is cleaner).

For simplicity, update the `whoami` and `static-content` services in your `docker-compose.yml` to remove those lines:

- For `whoami`:
  ```yaml
  labels:
    - "traefik.enable=true"
    - "traefik.http.routers.whoami.rule=Host(`whoami.local`)"
    - "traefik.http.routers.whoami.entrypoints=websecure"
    - "traefik.http.services.whoami.loadbalancer.server.port=80"
    - "traefik.http.routers.whoami.tls=true"
    # Remove: - "traefik.http.routers.whoami.tls.certresolver=localcert"
  ```

- For `static-content`:
  ```yaml
  labels:
    - "traefik.enable=true"
    - "traefik.http.routers.static.rule=Host(`static.local`)"
    - "traefik.http.routers.static.entrypoints=websecure"
    - "traefik.http.services.static.loadbalancer.server.port=80"
    - "traefik.http.routers.static.tls=true"
    # Remove: - "traefik.http.routers.static.tls.certresolver=localcert"
  ```

Traefik will automatically use the certificates from `dynamic-config.yml` based on the domain names.

---

### **Step 5: Configure Nginx (Optional)**

The `nginx` service is configured with ports 8081 (HTTP) and 8443 (HTTPS) and mounts a `conf.d` directory. By default, it serves static content from `./static-content`, but you can customize its behavior. For example, to serve content over HTTPS with the self-signed certificate:

Create `./nginx/conf.d/default.conf`:

```nginx
server {
    listen 80;
    server_name localhost;
    return 301 https://$host:$request_uri;  # Redirect HTTP to HTTPS
}

server {
    listen 443 ssl;
    server_name localhost;

    ssl_certificate /etc/nginx/certs/localhost.crt;
    ssl_certificate_key /etc/nginx/certs/localhost.key;

    location / {
        root /usr/share/nginx/html;
        index index.html;
    }
}
```

This configuration redirects HTTP to HTTPS and serves files from `./static-content`. Adjust the `server_name` or add proxy rules if you want Nginx to forward requests to Traefik or other services.

---

### **Step 6: Add Static Content**

Place a sample file in `./static-content` to test the `static-content` service and optionally the `nginx` service:

Create `./static-content/index.html`:

```html
<!DOCTYPE html>
<html>
<head>
    <title>Home Lab Static Content</title>
</head>
<body>
    <h1>Welcome to the Home Lab!</h1>
    <p>This is static content served by Nginx.</p>
</body>
</html>
```

---

### **Step 7: Update the Hosts File**

Since this is a local setup, map the domains `whoami.local` and `static.local` to `127.0.0.1` by editing your system's hosts file:

- **On Linux/macOS**:
  ```bash
  sudo nano /etc/hosts
  ```
  Add:
  ```
  127.0.0.1 whoami.local static.local
  ```

- **On Windows**:
  Edit `C:\Windows\System32\drivers\etc\hosts` (run your editor as Administrator) and add:
  ```
  127.0.0.1 whoami.local static.local
  ```

Save and exit.

---

### **Step 8: Start the Services**

From the directory containing your `docker-compose.yml`, run:

```bash
docker compose up -d
```

This starts all services in detached mode. Check the logs to ensure everything starts correctly:

```bash
docker compose logs
```

---

### **Step 9: Test the Setup**

- **Traefik Dashboard**: Open `http://localhost:8080` in your browser to view the Traefik dashboard (no authentication, safe for local use).
- **Whoami Service**: Visit `https://whoami.local`. Accept the self-signed certificate warning in your browser. You should see the Whoami service output (e.g., hostname, request details).
- **Static Content**: Visit `https://static.local`. Accept the certificate warning. You should see the `index.html` content from `./static-content`.
- **Nginx**: Visit `http://localhost:8081` or `https://localhost:8443`. If configured as above, you’ll see the same `index.html` or a custom Nginx setup.

---

### **Step 10: Customize and Expand**

- **Nginx Role**: Decide its purpose (e.g., direct static content, proxy to Traefik, or a separate web app). Adjust `./nginx/conf.d` accordingly.
- **Add Services**: Add more backend services to the `backend` network with Traefik labels for routing (e.g., a database or API).
- **Security**: For a production-like lab, replace self-signed certificates with Let's Encrypt by uncommenting and configuring the ACME lines in Traefik's `command`.

---

### **Summary**

This home lab infrastructure uses Docker Compose to manage:
- **Traefik** as the primary reverse proxy, routing requests to backend services (`whoami` and `static-content`) over HTTPS on ports 80 and 443.
- **Nginx** as an additional service on ports 8081 and 8443, configurable for static content or other purposes.
- **Local SSL certificates** for secure communication, with domains resolved via the hosts file.
- **Network isolation** with `frontend` and `backend` networks.

By following these steps, you’ll have a functional local home lab for testing and development!
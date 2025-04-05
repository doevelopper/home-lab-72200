# Homelab Service Access Reference

## Web Services Access Table

| Service | URL | Port (External) | Description | Authentication |
|---------|-----|----------------|-------------|----------------|
| Traefik Dashboard | `https://traefik.local` | 443 | Edge router management interface | Basic Auth (admin) |
| Nginx | `https://nginx.local` | 443 (or direct 8443) | Legacy reverse proxy | None by default |
| Static Content | `https://static.local` | 443 | Static file hosting | None by default |
| Whoami | `https://whoami.local` | 443 | Demo service showing request info | None |
| Portainer | `https://portainer.local` | 443 | Container management UI | Admin account required |
| Prometheus | `https://prometheus.local` | 443 | Metrics collection and storage | Basic Auth (admin) |
| Grafana | `https://grafana.local` | 443 | Metrics visualization dashboards | Admin account (default: admin/securepassword) |

## Direct Port Access

| Service | Host Port | Container Port | Protocol | Notes |
|---------|-----------|----------------|----------|-------|
| Traefik | 80 | 80 | HTTP | Redirects to HTTPS |
| Traefik | 443 | 443 | HTTPS | Primary web access |
| Traefik Metrics | 8082 | 8082 | HTTP | Internal only (monitoring network) |
| Nginx | 8081 | 80 | HTTP | Direct access to Nginx |
| Nginx | 8443 | 443 | HTTPS | Direct SSL access to Nginx |

## Network Configuration

* All web services are accessible through Traefik's HTTPS endpoint on port 443
* Services on the `frontend` network are externally accessible
* Services on the `backend` network are only accessible internally
* Services on the `monitoring` network are reserved for metrics collection

## Security Notes

* All web traffic is automatically redirected to HTTPS
* Security headers are applied to all HTTP responses
* Administrative interfaces require authentication
* Default credentials should be changed before deployment
* Domain suffix `.local` can be customized via the `LOCAL_DOMAIN` environment variable


Edit your hosts file:
Linux/Mac: /etc/hosts
Windows: C:\Windows\System32\drivers\etc\hosts

127.0.0.1 traefik.local nginx.local whoami.local static.local portainer.local prometheus.local grafana.local
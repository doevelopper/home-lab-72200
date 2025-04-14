SHELL := /bin/bash
DC = docker compose

help: ## Display this help.
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m<target>\033[0m\n"} /^[a-zA-Z_0-9-]+:.*?##/ { printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

##@ Docker

start: ## Start  docker-compose.yml
	$(DC) up -d --force-recreate --remove-orphans

stop: ## Stop docker-compose.yml
	$(DC) docker compose down  --remove-orphans

start-with-ollama: ## Start open-webui with ollama container docker-compose.ollama.yml
	$(DC) -f docker-compose.ollama.yml up -d

stop-with-ollama: ## Stop open-webui with ollama container docker-compose.ollama.yml
	$(DC) -f docker-compose.ollama.yml stop
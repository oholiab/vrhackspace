.PHONY: run clean build

default: run

run: build clean
	docker-compose run vrhs /workspace/main
	docker-compose down

clean:
	docker-compose down || true

build:
	docker-compose build

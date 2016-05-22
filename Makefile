default: run

run: clean
	docker-compose run vrhs /workspace/main
	docker-compose down

clean:
	docker-compose down

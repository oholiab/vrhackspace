DISPLAY?=:0

.PHONY: run

default: run

run: image
	cd src && make image
	sleep 5
	cd src && make runc &
	sleep 20
	docker run -it --volumes-from vrhs -e DISPLAY=$(DISPLAY) vrhs-dev

image:
	docker build -t vrhs-dev .

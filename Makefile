DISPLAY?=:0

.PHONY: run image

default: run

run: image
	cd src && make clean
	cd src && make image
	sleep 5
	cd src && make runc &
	sleep 5
	docker run -it --volumes-from vrhs -e DISPLAY=$(DISPLAY) vrhs-dev

image:
	docker build -t vrhs-dev .

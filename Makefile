DISPLAY?=:0

.PHONY: run

default: run

run: image
	cd src && make runc &
	docker run -it --volumes-from vrhs -e DISPLAY=$(DISPLAY) vrhs-dev

image:
	docker build -t vrhs-dev .

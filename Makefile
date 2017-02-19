do:

doc:
	cd text/
	make

receiver:
	cd src/receiver/ && make

sender:
	cd src/sender/ && make

run: receiver sender
	src/receiver/IBP
	src/sender/sender

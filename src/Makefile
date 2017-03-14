.PHONY:all
.PHONY:sender
.PHONY:receiver

all:
	+$(MAKE) -C	receiver 
	+$(MAKE) -C sender

sender:
	+$(MAKE) -C sender

receiver:
	+$(MAKE) -C receiver

clean:
	+$(MAKE) -C receiver clean
	+$(MAKE) -C sender clean

purge:
	+$(MAKE) -C receiver purge
	+$(MAKE) -C sender purge
	

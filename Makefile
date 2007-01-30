all:
clean:
%:
	cd common && $(MAKE) $@
	cd client && $(MAKE) $@
	cd server && $(MAKE) $@

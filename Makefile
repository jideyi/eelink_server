# Targets start here.
all clean:
	cd src && $(MAKE) $@

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all clean


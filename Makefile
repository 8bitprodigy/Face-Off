all:
	@echo "You must provide a platform as an argument!"
	@echo "Available arguments:\tdc\tdc_cdi\tlinux\n"
	@echo "Usage:"
	@echo "\tmake <argument>"

clean:
	@if [ "$(MAKECMDGOALS)" = "clean" ]; then \
		rm -rf obj; \
	fi

dc:
	$(MAKE) -f Makefile_dc $(filter-out dc,$(MAKECMDGOALS))
	
linux:
	$(MAKE) -f Makefile_linux $(filter-out linux,$(MAKECMDGOALS))
	
dc_cdi:
	@echo "Building DreamCast disc...\n"
	mkdcdisc -e face-off.dc.elf -o face-off.cdi

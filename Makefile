dc:
	make -f Makefile_dc
	
linux:
	make -f Makefile_linux
	
dc_cdi:
	echo "Building DreamCast disc...\n"
	mkdcdisc -e $(TARGET) -o $(PROJECTNAME).cdi

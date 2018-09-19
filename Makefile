include config.mk

.PHONY: clean

INCLUDE = -I ./mbedtls/include
LIBDIR = ./mbedtls/library
CFLAGS += -D_BSD_SOURCE -D_POSIX_SOURCE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE -D__USE_MINGW_ANSI_STDIO=1 -D_FILE_OFFSET_BITS=64

all:
	cd mbedtls && $(MAKE) lib
	$(MAKE) hacbrewpack

.c.o:
	$(CC) $(INCLUDE) -c $(CFLAGS) -o $@ $<

hacbrewpack: sha.o aes.o extkeys.o pki.o utils.o main.o filepath.o ConvertUTF.o nca.o romfs.o pfs0.o ivfc.o nacp.o cnmt.o
	$(CC) -o $@ $^ $(LDFLAGS) -L $(LIBDIR)

aes.o: aes.h types.h

extkeys.o: extkeys.h types.h settings.h

filepath.o: filepath.c types.h

main.o: main.c pki.h types.h version.h

pki.o: pki.h aes.h types.h

nca.o: nca.h

romfs.o: romfs.h

pfs0.o: pfs0.h

cnmt.o: cnmt.h

nacp.o: nacp.h

ivfc.o: ivfc.h

sha.o: sha.h types.h

utils.o: utils.h types.h

ConvertUTF.o: ConvertUTF.h

clean:
	rm -f *.o hacbrewpack hacbrewpack.exe

clean_full:
	rm -f *.o hacbrewpack hacbrewpack.exe
	cd mbedtls && $(MAKE) clean

dist: clean_full
	$(eval HACBREWPACKVER = $(shell grep '\bHACBREWPACK_VERSION\b' version.h \
		| cut -d' ' -f3 \
		| sed -e 's/"//g'))
	mkdir hacbrewpack-$(HACBREWPACKVER)
	cp -R *.c *.h config.mk.template Makefile README.md LICENSE mbedtls hacbrewpack-$(HACBREWPACKVER)
	tar czf hacbrewpack-$(HACBREWPACKVER).tar.gz hacbrewpack-$(HACBREWPACKVER)
	rm -r hacbrewpack-$(HACBREWPACKVER)


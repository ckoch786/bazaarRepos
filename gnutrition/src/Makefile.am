## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(sqlite3_CFLAGS) \
	$(GNUTRITION_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gnutrition

gnutrition_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c \
	recipe_tab.c \
        user_interface \
	user_interface.c \
	user_interface.h \
	sqlite3_api.c \
	sqlite3_api.h \
	sqlite3.c \
	sqlite3.h \
	sqlite3ext.h \
	../sqlite3.pc \
	../sqlite3.pc.in \
	food_db_tab.c \
	file_handling.c \
	preferences_tab.c \
	food_journal_tab.c 

gnutrition_LDFLAGS = \
	-Wl,--export-dynamic

gnutrition_LDADD = $(sqlite3_LIBS) \
	$(GNUTRITION_LIBS)


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(datadir)/gnutrition

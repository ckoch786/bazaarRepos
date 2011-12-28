/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
 * Copyright (C) 2011 Free Software Foundation, Inc.
 *
 * This file is part of GNUtrition.
 * 
 * GNUtrition is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * GNUtrition is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <config.h>

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include "user_interface.h"

int main (int argc, char *argv[])
{
	
  if (argc == 2)
  {
	if (strcmp(argv[1], "--help") == 0)
    {
	  printf( _("Report bugs to: "));
      printf("bug-gnutrition@gnu.org\n");
      printf( _("GNUtrition home page: "));
      printf( "<http://www.gnu.org/software/gnutrition/>\n");
	  printf( _("General help using GNU software: "));
      printf("<http://www.gnu.org/gethelp/>\n");
    }

    else if (strcmp(argv[1], "--version") == 0)
    {
	  printf("GNUtrition 0.4\n");
      printf( _("Copyright (C) 2010, 2011 Free Software Foundation, Inc.\n"
       "License GPLv3+: GNU GPL version 3 or later "));
      printf("<http://gnu.org/licenses/gpl.html>\n");
      printf( _("This is free software: you are free to change and redistribute it.\n"
       "There is NO WARRANTY, to the extent permitted by law.\n"));
    }
  }

  else if (argc == 1)
  {
#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

	UI_initialize(&argc, argv);
  }

  else
  {
	printf("Invalid number of command line arguments\n");
  }

  return 0;
}

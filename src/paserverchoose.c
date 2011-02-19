/***
  This file is part of paserverchoose.

  paserverhoose is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 2 of the License,
  or (at your option) any later version.

  padevchooser is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with padevchooser; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <argp.h>
#include <avahi-client/lookup.h>

#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>

#include <X11/Xlib.h>

#include "avahibrowse.h"
#include "x11prop.h"

#define P_SERVER "PULSE_SERVER"
#define P_ID "PULSE_ID"

const char *argp_program_version = "paserverchoose 0.1";
const char *argp_program_bug_address = "<brendan@fridu.net>";
static char doc[] = "paserverchoose -- a cli client to change the pulseaudio server";
/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 [STRING...]";

/* The options we understand. */
static struct argp_option options[] = {
    {"display", 'd', 0, 0, "Display current pulseaudio server ", 0},
    {"list", 'l', 0, 0, "List all pulseaudio servers available ", 0},
    {"server", 's', 0, 0, "Set pulseaudio server", 0},
    {"reset", 'r', 0, 0, "Reset the pulseaudio server to use the local client", 0},
    {0, 0, 0, 0, 0, 0}
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *args[2];                /* arg1 & arg2 */
    int display, list, reset;
    char *server;
};

static gchar* get_pulse_server(Display *display) {

    char t[256];
    char *current_server = NULL;

    current_server = g_strdup(x11_get_prop(display, P_SERVER, t, sizeof(t)));

    return current_server;
}

static void set_pulse_server(Display *display, gchar *server) {

    if (server)
        x11_set_prop(display, P_SERVER, server);
    else
        x11_del_prop(display, P_SERVER);

    /* This is used by module-x11-publish to detect whether the
     * properties have been altered. We delete this property here to
     * make sure that the module notices that it is no longer in
     * control */
    x11_del_prop(display, P_ID);
}

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
     * know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

     switch (key) {
         case 'd':
             arguments->display = 1;
             break;
         case 'l':
             arguments->list = 1;
             break;
         case 's':
             arguments->server = state->argv[state->next];
             state->next = state->argc;
             break;
         case 'r':
             arguments->reset = 1;
             break;
         default:
             return ARGP_ERR_UNKNOWN;
       }
       return 0;
}

/* The Parser */
static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[]) {

    Display *display;
    struct arguments arguments;

    /* Default values. */
    arguments.display = 0;
    arguments.list = 0;
    arguments.reset = 0;
    arguments.server = NULL;

    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Could not open display. Are you sure you are running this in X?");
        return -1;
    }

    /* Set PA server */
    if (arguments.server) {
        set_pulse_server(display, arguments.server);
        fprintf (stdout, "Pulseaudio server set to %s\n", arguments.server);
    }
    /* Reset PA server to default client */
    else if (arguments.reset) {
        set_pulse_server(display, NULL);
    }
    /* List PA servers */ 
    else if (arguments.list) {
        find_pulse_servers();
    }
    /* Get current PA server */
    else {
        fprintf (stdout, "Current pulseaudio server is : %s\n", get_pulse_server(display));
    }

    return 0;
}

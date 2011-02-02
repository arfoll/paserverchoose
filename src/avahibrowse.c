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

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>

#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>

#define A_SERVICE_TYPE "_pulse-server._tcp"

static AvahiSimplePoll *simple_poll = NULL;

static int done = 0;
static int services = 0;

static void resolve_callback(
    AvahiServiceResolver *r,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AvahiResolverEvent event,
    const char *name,
    const char *type,
    const char *domain,
    const char *host_name,
    const AvahiAddress *address,
    uint16_t port,
    AvahiStringList *txt,
    AvahiLookupResultFlags flags,
    AVAHI_GCC_UNUSED void* userdata) {

    assert(r);

    switch (event) {
        case AVAHI_RESOLVER_FAILURE:
            fprintf(stderr, "(Resolver) Failed to resolve service '%s' of type '%s' in domain '%s': %s\n", name, type, domain, avahi_strerror(avahi_client_errno(avahi_service_resolver_get_client(r))));
            break;

        case AVAHI_RESOLVER_FOUND: {
            char a[AVAHI_ADDRESS_STR_MAX];

            fprintf(stdout, "Service '%s' of type '%s' in domain '%s':\n", name, type, domain);

            avahi_address_snprint(a, sizeof(a), address);
            fprintf(stdout, "\t%s:%u (%s)\n", host_name, port, a);
        }
    }

    avahi_service_resolver_free(r);
}

static void browse_callback(
    AvahiServiceBrowser *b,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiBrowserEvent event,
    const char *name,
    const char *type,
    const char *domain,
    AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
    void* userdata) {

    AvahiClient *c = userdata;
    assert(b);

    switch (event) {

        case AVAHI_BROWSER_FAILURE:
            fprintf(stderr, "Unknown avahi_browser failure.%s\n",
                avahi_strerror(avahi_client_errno(
                avahi_service_browser_get_client(b))));
            avahi_simple_poll_quit(simple_poll);
            return;

        case AVAHI_BROWSER_NEW:
            services++;
            fprintf(stderr, "Service '%s' of type '%s' in domain '%s'\n", 
                name, type, domain);
            break;

        case AVAHI_BROWSER_ALL_FOR_NOW:
            done = 1;

        default:
            break;
    }
}

void find_pulse_servers() {
    AvahiClient *client = NULL;
    int error, i;

    /* Allocate main loop object */
    simple_poll = avahi_simple_poll_new(); 

    /* Allocate a new client */
    client = avahi_client_new(avahi_simple_poll_get(simple_poll), 0, 
        NULL, NULL, &error);

    avahi_service_browser_new(client, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 
        A_SERVICE_TYPE, NULL, 0, browse_callback, client);

    /* Discover all the services until AVAHI_BROWSER_ALL_FOR_NOW */
    while (!done) {
        if (avahi_simple_poll_iterate(simple_poll, NULL) != 0)
            break;       
    }

    /* Display result for all servers found */
    for (i = 0; i < services; i++) {
        if (avahi_simple_poll_iterate(simple_poll, NULL) != 0)
            break;
    }
}

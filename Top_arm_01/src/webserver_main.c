/*
 * Copyright (c) 2007-2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>

#include "xmk.h"
#include "xenv_standalone.h"
#include "xparameters.h"

#include "platform_config.h"
#include "platform.h"

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/init.h"
#include "netif/xadapter.h"

#include "config_apps.h"
#include "mfs_config.h"
#include "app.h"

#define PLATFORM_EMAC_BASEADDR  XPAR_EMACLITE_0_BASEADDR

void
print_ip(char *msg, struct ip_addr *ip)
{
    print(msg);
    xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
            ip4_addr3(ip), ip4_addr4(ip));
}

void
print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

    print_ip("Board IP: ", ip);
    print_ip("Netmask : ", mask);
    print_ip("Gateway : ", gw);
}

struct netif server_netif;

void network_thread(void *p)
{
    struct netif *netif;
    struct ip_addr ipaddr, netmask, gw;

    /* the mac address of the board. this should be unique per board */

    unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

    netif = &server_netif;

    // initliaze IP addresses to be used
    IP4_ADDR(&ipaddr,  192, 168,   1, 10);
    IP4_ADDR(&netmask, 255, 255, 255,  0);
    IP4_ADDR(&gw,      192, 168,   1,  1);

    // print out IP settings of the board
    print("\n\r\n\r");
    //print("-----lwIP Socket Mode Demo Application ------\n\r");
    print("IP Settings\n\r");
    print_ip_settings(&ipaddr, &netmask, &gw);

    // print all application headers
    print_headers();

    // Add network interface to the netif_list, and set it as default
    if (!xemac_add(netif, &ipaddr, &netmask, &gw,
        mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
        xil_printf("Error adding N/W interface\n\r");
        return;
    }
    netif_set_default(netif);

    /* specify that the network if is up */
    netif_set_up(netif);

    /* start packet receive thread - required for lwIP operation */
    sys_thread_new("xemacif_input_thread",
            (void(*)(void*))xemacif_input_thread, netif,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);

    /* now we can start application threads */
    launch_app_threads();

    return;
}

/*
 * Initial thread entrypoint
 */
int
main_thread()
{
	unsigned char inchar;
	/* initialize lwIP before calling sys_thread_new */
    xil_printf("XAPPINFO:Initialize LWIP\n\r");
    lwip_init();

    //analytics_init();

    apm_core();

    init_http_mutexes();

    // any thread using lwIP should be created using sys_thread_new
    //xil_printf("create network_thread\n\r");

    sys_thread_new("NW_THREAD", network_thread, NULL,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);

    /* now we can start application threads */
//     launch_app_threads();


     //perf_help_web();
     //apm_core();
     while (1)
     {
        print(">");
        inchar = inbyte();
        xil_printf("%c\n\r",inchar);
        switch (inchar)
        {
           case 0x1b :
           case 'q' :
              xil_printf("- exit menu -\n\n\r");
              //analytics_menu();
              perf_mon_main();
              //return;
              break;
           case '?' :
           {
        	   //perf_help_web();
        	   perf_mon_main();
              break;
           }
           case 'R' :
           {


              break;
           }
        }
     }



    return 0;
}


void perf_help_web(void)
{
  print("\n\r");
  print("-----------------------------------------------------\n\r");
  print("-- Layer Switcher/Performance Menu                 --\n\r");
  print("-----------------------------------------------------\n\r");
  print("\n\r");
  print(" Select option\n\r");
  print(" 0 = Layer 0 Only - Colorbars\n\r");
  print(" 1 = Layer 1 Only - Zoneplates\n\r");
  print(" 2 = Layer 2 Only - Vertical Sweep\n\r");
  print(" 3 = Layer 3 Only - Horizontal Sweep\n\r");
  print(" 4 = Layer 4 Only - Colorbars\n\r");
  print(" 5 = Alpha blend (All Layers)\n\r");
  print(" 6 = Benchmark Design (Average)\n\r");
  print(" 7 = Benchmark Design (Single Frame)\n\r");
  print("\n\r");
  print(" q = Perf Mon Application \n\r");
  print(" ?  = help \n\r");
  print("-----------------------------------------------------\n\r");
}


int
main()
{

    if (init_platform() < 0) {
        xil_printf("ERROR initializing platform.\n\r");
        return -1;
    }

    //* start the kernel - does not return /
    xil_printf("XAPPINFO:Starting XILKERNEL.\n\r");
    xilkernel_main();

    return 0;
}


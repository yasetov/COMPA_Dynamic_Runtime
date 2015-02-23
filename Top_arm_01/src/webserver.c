/*
 * Copyright (c) 2007, 2009 Xilinx, Inc.  All rights reserved.
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
#include <string.h>

#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwipopts.h"

#include "webserver.h"

static unsigned http_port = 80;


enum http_req_type
{ HTTP_GET, HTTP_POST, HTTP_UNKNOWN };

enum http_req_type
decode_http_request(char *req, int l)
{
    char *get_str = "GET";
    char *post_str = "POST";

    if (!strncmp(req, get_str, strlen(get_str)))
        return HTTP_GET;

    if (!strncmp(req, post_str, strlen(post_str)))
        return HTTP_POST;

    return HTTP_UNKNOWN;
}

/* generate and write out an appropriate response for the http request */
int generate_response(int sd, char *http_req, int http_req_len)
{
    enum http_req_type request_type =
        decode_http_request(http_req, http_req_len);

    switch (request_type) {
    case HTTP_GET:
//        return do_http_get(sd, http_req, http_req_len);
    case HTTP_POST:
//        return do_http_post(sd, http_req, http_req_len);
    default:
//        return do_404(sd, http_req, http_req_len);
    }
}

/* thread spawned for each connection */
void process_http_request(int sd)
{
    int read_len;
    /* since these buffers are allocated on the stack .. */
    /* .. care should be taken to ensure there are no overflows */
    char recv_buf[RECV_BUF_SIZE];

    /* read in the request */
    if ((read_len = read(sd, recv_buf, RECV_BUF_SIZE)) < 0) return;
    //xil_printf("at http req\r\n");
    /* respond to request */
    generate_response(sd, recv_buf, read_len);

    /* close connection */
    close(sd);
}

/* http server */
int web_application_thread()
{
    int sock, new_sd;
    struct sockaddr_in address, remote;
    socklen_t size;

    /* create a TCP socket */
    if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return 0;

    /* bind to port 80 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(http_port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (lwip_bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0)
        return 0;

    /* listen for incoming connections */
    lwip_listen(sock, 5);

    size = sizeof(remote);
    while (1) {
    	sys_thread_t new_thread;

        new_sd = lwip_accept(sock, (struct sockaddr *) &remote, &size);
//        //xil_printf("Start Thread for http requests \n\r");
//        /* spawn a separate handler for each request */
//        new_thread = sys_thread_new("httpd", (void (*)(void *)) process_http_request,
//                       (void *) new_sd, THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
//
//        /* close connection if no new threads available */
//        if (new_thread == 0) {
//            close(new_sd);
//        }
        process_http_request(new_sd);
    }

    return 0;
}


void print_web_app_header()
{
    xil_printf("%20s %6d %s\n\r", "http server", http_port, "http://192.168.1.10");
}

void readPerfMonitor(XAxiPmon *AxiPmonInstPtr,
					u32 Metrics[],
					u32 *ClkCntHigh,
					u32 *ClkCntLow){

	u8 SlotId = 0x0;
	/*
	 * Disable Global Clock Counter Register.
	 */

	XAxiPmon_DisableGlobalClkCounter(AxiPmonInstPtr);

	/*
	 * Disable Metric Counters.
	 */
	XAxiPmon_DisableMetricsCounter(AxiPmonInstPtr);

	/* Get Metric Counters  */
	for (SlotId = 0; SlotId < AxiPmonInstPtr->Config.NumberofSlots; SlotId++) {
		Metrics[SlotId] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, SlotId);
//		Metrics[SlotId][1] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_1);
//		Metrics[SlotId][2] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_2);
//		Metrics[SlotId][3] = XAxiPmon_GetMetricCounter(AxiPmonInstPtr, XAPM_METRIC_COUNTER_3);

//		Incrementers[SlotId][0] = XAxiPmon_GetIncrementer(AxiPmonInstPtr, XAPM_INCREMENTER_0);
	}

	/* Get Global Clock Cycles Count in ClkCntHigh,ClkCntLow */
	XAxiPmon_GetGlobalClkCounter(AxiPmonInstPtr, ClkCntHigh, ClkCntLow);


}

void printMetricValues(XAxiPmon *AxiPmonInstPtr, u8 nbSlots, u8 Metric){
	u8 SlotId;
	u32 ClkCntHigh;
	u32 ClkCntLow;
	u32 Metrics[MAX_NUM_MONITOR_SLOTS] = {0};

	// Clu
	readPerfMonitor(AxiPmonInstPtr, Metrics, &ClkCntHigh, &ClkCntLow);
	for (SlotId = 0; SlotId < nbSlots; SlotId++) {
		xil_printf("***APM_%d***\r\n", AxiPmonInstPtr->Config.DeviceId);
		xil_printf("%s : %d\r\n", XAxiPmon_GetMetricName(Metric), Metrics[SlotId]);
	}
}


void create_chart_data_str(char* dataStr){
	/* Java Script literal data syntax.
  var data = google.visualization.arrayToDataTable([
    ['Year', 'Sales', 'Expenses'],
    ['2004',  1000,      400],
    ['2005',  1170,      460],
    ['2006',  660,       1120],
    ['2007',  1030,      540]
  ]);


	*/
	/*** Print metric values for each slot on PMon system ***/
//		printMetricValues(AxiPmonInst_00, nbslots, Metrics, &ClkCntHigh, &ClkCntLow);
//		readPerfMonitor(AxiPmonInstPtr, Incrementers, Metrics, &ClkCntHigh, &ClkCntLow);
//		for (SlotId = 0; SlotId < XPAR_AXIPMON_2_NUM_MONITOR_SLOTS; SlotId++) {
//			xil_printf("***APM_%d***\r\n", AxiPmonInstPtr->Config.DeviceId);
//			xil_printf("%s : %d\r\n", XAxiPmon_GetMetricName(XAPM_METRIC_SET_1), Metrics[SlotId]);
//		}
	// Print metric values for each slot on PMon 00
//	printMetricValues(&AxiPmonInst_00, XPAR_AXIPMON_0_NUM_MONITOR_SLOTS, XAPM_METRIC_SET_0);
//
//	// Print metric values for each slot on PMon 10
//	printMetricValues(&AxiPmonInst_10, XPAR_AXIPMON_1_NUM_MONITOR_SLOTS, XAPM_METRIC_SET_0);
//
//	// Print metric values for each slot on PMon 01
//	printMetricValues(&AxiPmonInst_01, XPAR_AXIPMON_2_NUM_MONITOR_SLOTS, XAPM_METRIC_SET_1);
//
//	// Print metric values for each slot on PMon 11
//	printMetricValues(&AxiPmonInst_11, XPAR_AXIPMON_3_NUM_MONITOR_SLOTS, XAPM_METRIC_SET_1);
	/******/
}


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

#include <string.h>

#include "xstatus.h"

#include "pthread.h"
#include "mfs_config.h"
#include "lwip/sys.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "sys/process.h"

#include "webserver.h"
//#include "platform_fs.h"
//#include "platform_gpio.h"
//#include "cdma_intr.h"

#define GET_BUFSIZE  4096
#define BUFSIZE      1024
#define NUMBER_OF_DMAS 	15
/*
 * Maximum number of benchmark data points to post at one time
 */
#define MAX_BENCHMARK_DATA 10

/*
 * Provide mutual exclusion for CDMA benchmark data POST
 */
sys_sem_t cdma_mutex;

/*
 * Store the benchmark data points and count the number of benchmark
 * data points currently stored.
 */
//double *benchmark_databuf;
//int count_benchmark_data = 0;

char *notfound_header = "<html> \
	<head> \
		<title>404</title> \
  		<style type=\"text/css\"> \
		div#request {background: #eeeeee} \
		</style> \
	</head> \
	<body> \
	<h1>404 Page Not Found</h1> \
	<div id=\"request\">";

char *notfound_footer = "</div> \
	</body> \
	</html>";

char *redirect_home = "<html> \
         <head> \
           <meta HTTP-EQUIV=\"REFRESH\" content=\"0; url=index.html\"> \
         </head> \
         </html> ";

char *js_header = "<html> \
	<head> \
		<title>LEDs</title> \
  		<style type=\"text/css\"> \
		div#request {background: #eeeeee} \
		</style> \
	</head> \
	<body> \
	<h1>LEDs Value</h1> \
	<div id=\"request\"> \
	<script type=\"text/javascript\"> \
	var switchval; \
	switchval = \"";

char *js_footer = "\" \
	document.write(switchval); \
	</script> \
	</div> \
	</body> \
	</html>";

/*
 * Provide a thread-safe memory allocation
 * NOTE: consider LWIP mem_malloc(), mem_free()
 */
static void *
safe_malloc (size_t size)
{
    void *p;

    xmk_enter_kernel();
    p = malloc(size);
    xmk_leave_kernel();

    return p;
}

/*
 * Provide a thread-safe memory allocation
 */
static void
safe_free (void *ptr)
{
    xmk_enter_kernel();
    free(ptr);
    xmk_leave_kernel();
}

/*
 * Allocate the mutexes used handling HTTP requests.
 * Implemented with the LWIP sys_sem facilities.
 */
void
init_http_mutexes()
{
    cdma_mutex = sys_sem_new(&cdma_mutex, 1);
}

/* dynamically generate 404 response:
 *	this inserts the original request string in betwween the notfound_header & footer strings
 */
int
do_404(int sd, char *req, int rlen)
{
    int len, hlen;
    char buf[BUFSIZE];

    len = strlen(notfound_header) + strlen(notfound_footer) + rlen;

    hlen = generate_http_header(buf, "html", len);
    if (lwip_write(sd, buf, hlen) != hlen) {
        xil_printf("error writing http header to socket\n\r");
        xil_printf("http header = %s\n\r", buf);
        return -1;
    }

    lwip_write(sd, notfound_header, strlen(notfound_header));
    lwip_write(sd, req, rlen);
    lwip_write(sd, notfound_footer, strlen(notfound_footer));

    return 0;
}

int
is_cmd_led(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/ledxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ledxhr", 6));
}

int
is_cmd_switch(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/switchxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "switchxhr", 9));
}

int
is_cmd_cdma(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "cdmaxhr", 7));
}

int is_cmd_hltp(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "hltp", 4));
}

int is_cmd_atg0(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "atg0", 4));
}
int is_cmd_atg1(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "atg1", 4));
}
int is_cmd_atg2(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "atg2", 4));
}
int is_cmd_atg3(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "atg3", 4));
}
int is_cmd_hlt0(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "hlt0", 4));
}
int is_cmd_ads0(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ads0", 4));
}
int is_cmd_ads1(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ads1", 4));
}
int is_cmd_ads2(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ads2", 4));
}
int is_cmd_ads3(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ads3", 4));
}
int is_cmd_ads4(char *buf)
{
        /* skip past 'POST /' */
        buf += 6;

        /* then check for cmd/cdmaxhr */
        return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ads4", 4));
}
/*
 * Copy the complete next HTTP header line from the mondo input string to 'out'.
 * Reads from the socket if needed.
 */
static int
get_next_line (int sd, char *out, char *in, int *inindexp, int *inbuflenp)
{
    int dsti;

    if (*inbuflenp - *inindexp < 2) {
        xil_printf("%s: remaining buflen %d - %d\n\r", __FUNCTION__,
            *inbuflenp, *inindexp);
    }

    /* End of headers? */
    if ((in[*inindexp] == '\r') &&
        (in[*inindexp + 1] == '\n')) {
       return 0;
    }

    dsti = 0;
    while (in[*inindexp] && (in[*inindexp] != '\n')) {
        /* Copy input buf to output buf */
        out[dsti] = in[*inindexp];
        (*inindexp)++;

        /* Don't overflow the output buffer */
        if (dsti < BUFSIZE - 2) {
            dsti++;
        }

        /* Refill the input buffer */
        if ((*inbuflenp - *inindexp) <= 0) {
            *inbuflenp = read(sd, in, RECV_BUF_SIZE);
            *inindexp = 0;
            if (*inbuflenp <= 0) {
                xil_printf("%s: Insufficient socket data.\n\r", __FUNCTION__);
                return 0;
            }
        }
    }
    if (dsti >= BUFSIZE - 2) {
        xil_printf("%s: Header larger than BUFSIZE (truncated).\n\r",
            __FUNCTION__);
    }

    (*inindexp)++;
    out[dsti] = 0;

    return (dsti);
}

/*
 * Fish out an integer field in the header info, such as "Content-Length:"
 */
static int
get_http_header_int_val (char **http_headers, const char *which)
{
    int linelen;
    const char *header;
    int i;

    if (http_headers == 0) {
        xil_printf("%s: NULL http_headers\n\r", __FUNCTION__);
        return 0;
    }

    i = 0;
    while (http_headers[i]) {
        header = http_headers[i];
        if (strncmp(which, header, strlen(which)) == 0) {
            linelen = atoi(&header[strlen(which)]);
            return linelen;
        }
        i++;
    }

    return 0;
}

/*
 * Fetch data POSTed to the server; place it in *databuf provided by caller.
 */
static int
receive_post_data (int sd, char *in, int rlen,
                   char *databuf, int datalen, int skip_http_hdr)
{
    int i, read_len, http_hdr, hdr_bytes;

    /* Skip the request header to get to any data received in the
     * first buffer. It terminates with \n\r\n. Any additional headers
     * are counted against the Content-Length, and the header size should
     * be subtracted from the amount of client data to read.
     */
    i = 0;
    http_hdr = 0;
    hdr_bytes = 0;
    while (http_hdr < skip_http_hdr) {
        /* End of this header ? */
        while (!((in[i] == '\n') && (in[i + 1] == '\r') &&
                 (in[i + 2] == '\n'))) {
            if ((i + 3) > rlen) {
                /* End of buffer data; End of header not encountered yet.
                 * Read more data.
                 */
                i = 0;
                rlen = read(sd, in, RECV_BUF_SIZE);
                if (rlen <= 0) {
                    xil_printf("receive_post_data: insufficient data.\n\r");
                    return 0;
                }
            } else {
                i++;
                hdr_bytes++;
            }
        }
        hdr_bytes += 3;
        i += 3;

        if (http_hdr > 0) {
            /* Additional headers count against the Content-Length specified
             * in the first header.
             */
            datalen -= hdr_bytes;
        }

        /* The remainder of this buffer belongs to the next header */
        http_hdr++;
        hdr_bytes = rlen - i;
    }

    /* Copy any data bytes present in the current buffer */
    rlen -= i;
    memcpy(databuf, &in[i], rlen);
    databuf += rlen;
    datalen -= rlen;

    /* read the remainder */
    while (datalen > 0) {
        read_len = read(sd, databuf, datalen);
        if (read_len < 0) {
            xil_printf("read: %d (datalen %d)\n\r", read_len, datalen);
            return read_len;
        }
        databuf += read_len;
        datalen -= read_len;
    }
    *databuf = 0;

    return 0;
}

//#define MAX_HTTP_HEADERS  30
#define MAX_HTTP_HEADERS  200

/*
 * Returns an array of all the HTTP headers.
 */
static char **
http_parse_headers (int sd, char *req, int *rlen)
{
    char *line;
    int linelen;
    int indexp;
    char **http_headers;
    int total_headers;

    http_headers = 0;
    line = safe_malloc(BUFSIZE);
    if (line == 0) {
        xil_printf("Unable to allocate buffer-H1.\n\r");
        return http_headers;
    }
    http_headers = safe_malloc(sizeof(char*) * (MAX_HTTP_HEADERS + 1));
    if (http_headers == 0) {
        xil_printf("Unable to allocate header space.\n\r");
        safe_free(line);
        return http_headers;
    }
    total_headers = 0;

    indexp = 0;
    while (total_headers < MAX_HTTP_HEADERS) {
        /* Fetch a header line */
        linelen = get_next_line(sd, line, req, &indexp, rlen);
        if (linelen) {
            /* Copy this header line into an array */
            http_headers[total_headers] = safe_malloc(linelen + 4);
            if (http_headers[total_headers]) {
                strncpy(http_headers[total_headers], line, linelen + 2);
            } else {
                xil_printf("Unable to allocate HTTP header %d buffer(%d)\n\r",
                           total_headers, linelen);
                safe_free(line);
                return 0;
            }
            total_headers++;
        } else {
            break;
        }
    }
    if (total_headers == MAX_HTTP_HEADERS) {
        xil_printf("%s: total_headers = MAX_HTTP_HEADERS\n\r", __FUNCTION__);
    }

    safe_free(line);
    http_headers[total_headers] = 0;
    return(http_headers);
}

/*
 * Free header array obtained with http_parse_headers()
 */
static void
http_free_headers (char **http_headers)
{
    int i;

    if (http_headers == 0) {
        xil_printf("%s: NULL http_headers\n\r", __FUNCTION__);
        return;
    }

    i = 0;
    while (http_headers[i]) {
        safe_free(http_headers[i]);
        http_headers[i] = 0;
        i++;
    }
    safe_free(http_headers);
}

/*
 * Display header array obtained with http_parse_headers()
 */
static void
print_http_headers (char **http_headers)
{
    int i;

    if (http_headers == 0) {
        return;
    }

    xil_printf("HTTP HEADERS:\n\r");
    xil_printf("========================================\n\r");
    i = 0;
    while (http_headers[i]) {
        xil_printf("%d: %s\n\r", i, http_headers[i]);
        i++;
    }
    xil_printf("========================================\n\r");
}

/*
 * Toggle LEDs
 */
/*
static int
http_post_led (int sd, char *req, int rlen)
{
    int    datalen, len, hlen;
    char   buf[BUFSIZE];
    char string[100];
    char **http_headers;
    char *data;
    char *databuf;

    http_headers = http_parse_headers(sd, req, &rlen);
//	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }

	databuf = (char *) safe_malloc(datalen + 20);
	data = (char *) safe_malloc(datalen + 20);

	if(databuf) {
		receive_post_data (sd, req, rlen, databuf, datalen, 1);
		data = strtok(databuf, "=");
		data = strtok(NULL, "\r");
		set_leds(data);
	}
	safe_free(databuf);
    len = sprintf(string, "LEDs are now set to 0x%s\n", data);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, string, strlen(string));

	xil_printf("http POST: ledstatus: %s\n\r", data);
	safe_free(data);

	len = 0;

    return 0;
}
*/
/*
 * Display the switch values
 */
/*
static int
http_post_switch (int sd, char *req, int rlen)
{
    int    datalen, len;
    char   buf[BUFSIZE];
    char **http_headers;
    int n_switches = 8;
    int n;
    char *p;
    unsigned s;

    http_headers = http_parse_headers(sd, req, &rlen);
//	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }

	s = get_switch_state();
    xil_printf("http POST: switch state: %x\n\r", s);
 	len = generate_http_header(buf, "txt", n_switches);
    p = buf + len;
    for (n = 0; n < n_switches; n++) {
        *p++ = '0' + (s & 0x1);
        s >>= 1;
    }
    len += n_switches;

    lwip_write(sd, buf, len);

    len = 0;

    return 0;
}
*/
/*
 * Display the benchmark values
 */

int get_param(char* req, int p_index)
{
	int i;
	int pos = 0;

	for(i=0; req[i] != '\0'; i++)
	{
		if(req[i] == ',')
		{
			pos = atoi( (req+i+1) );
			if(p_index == 0) break;
			p_index--;
		}
	}

	//memset(logbuf, 0, sizeof(logbufr) );
	return pos;
}


static int http_post_hltp (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int hltp_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    hltp_sel = get_param(req, 0);	// live selection

    //xil_printf("===>>> http_post_hltp: 0x%x <<<===\r\n", hltp_sel);
    set_xapp_mode(hltp_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}

static int http_post_atg0 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int atglen_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    atglen_sel = get_param(req, 0);	// live selection

    //xil_printf("ATG0 Setting: 0x%x\r\n", atglen_sel);
    config_atg0(atglen_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_atg1 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int atglen_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    atglen_sel = get_param(req, 0);	// live selection
    config_atg1(atglen_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_atg2 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int atglen_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    atglen_sel = get_param(req, 0);	// live selection
    config_atg2(atglen_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_atg3 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int atglen_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    atglen_sel = get_param(req, 0);	// live selection
    config_atg3(atglen_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_hlt0 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int hlten_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    hlten_sel = get_param(req, 0);	// live selection
    config_hlt0(hlten_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_ads0 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int ads_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    ads_sel = get_param(req, 0);	// live selection
    config_ads0(ads_sel);
    //xil_printf("resp0\r\n");
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_ads1 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int ads_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    ads_sel = get_param(req, 0);	// live selection
    config_ads1(ads_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_ads2 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int ads_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    ads_sel = get_param(req, 0);	// live selection
    config_ads2(ads_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_ads3 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int ads_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    ads_sel = get_param(req, 0);	// live selection
    config_ads3(ads_sel);
    //xil_printf("resp3\r\n");
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}
static int http_post_ads4 (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    int ads_sel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    ads_sel = get_param(req, 0);	// live selection
    config_ads4(ads_sel);
	len = strlen("ok");
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, "ok", 2);

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}



/*
 * Display the benchmark values
 */
static int
http_post_cdma (int sd, char *req, int rlen)
{
	int status;
	double *resultbuf;
    int    datalen, len, hlen;
    int    len1, len2;
    char   buf[BUFSIZE];
    char **http_headers;
    char string[100];
    char string1[100];

    int str_len [NUMBER_OF_DMAS];
    int string_http [NUMBER_OF_DMAS] [100];
    int i;

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutex, 0);

    http_headers = http_parse_headers(sd, req, &rlen);
	//print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }

	resultbuf = (double *) safe_malloc(NUMBER_OF_DMAS*20);

	//xil_printf("res_buf 0x%x\n\r",&resultbuf);
	//perf_main_web(1280, 720, resultbuf);                      //****************************************
	perf_mon_web(resultbuf);                      //****************************************
	//status = SimpleIntrExample(resultbuf, 1);
	if(status == XST_FAILURE) {
		xil_printf("Error getting benchmarking data \n\r");
	}
/*
	resultbuf[0] = 0;

	sprintf(string, "%f", resultbuf[0]);
	sprintf(string1, "%f", resultbuf[1]);

	//len = strlen(string);
	len1  = strlen(string);
	len2 = strlen(string1);

	len = len1+len2;

	if (len1 <10) len ++;
	if (len2 <10) len ++;

	safe_free(resultbuf);
    printf("http POST: benchmark data-0: %s\n\r", string);
    printf("http POST: benchmark data-1: %s\n\r", string1);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
 	if (len1 <10) lwip_write(sd, "0", 1);
    lwip_write(sd, string, strlen(string));
    //lwip_write(sd, " ", 1);
    lwip_write(sd, string1, strlen(string1));

    len = 0;
    hlen = 0;
*/

	resultbuf[NUMBER_OF_DMAS] = 0;


	for (i=0; i< NUMBER_OF_DMAS; i++)

	{
		sprintf(string_http[i], "%f", resultbuf[i]);
		str_len[i] = strlen(string_http[i]);
		if (str_len[i] < 10)
		{
			len++;
		}
		len = len + str_len[i];
	}

	for (i=0; i< NUMBER_OF_DMAS; i++)
	{
	  //printf("http POST: benchmark data-%d: %s\n\r", i,string_http[i]);

		if (i==0)
		{
			len++; // Added to send the info about no of DMA present in the system
			hlen = generate_http_header(buf, "svg", len);
		 	lwip_write(sd, buf, hlen);
		 	// Throughput channels
		 	lwip_write(sd, "15", 2);
		 	// Latency channels
		 	//lwip_write(sd, "1", 1);
		}
		if (i==8) {
                 //xil_printf("strlen %d\n\r",str_len[i]);
                }

		if (str_len[i] < 10)
		{
			lwip_write(sd, "0", 1);
		}
		if (str_len[i] < 9)
		{
			lwip_write(sd, "0", 1);
		}
		if (str_len[i] > 10)
		{
                 //xil_printf("gt10 at i = %d\n\r",i);
		}
		if (str_len[i] < 8)
		{
                 //xil_printf("lt8 \n\r");
		}


		lwip_write(sd, string_http[i], str_len[i]);

	}


    len = 0;
    hlen = 0;

    safe_free(resultbuf);
    /* unlock mutex */
    sys_sem_signal(cdma_mutex);

    return 0;
}

/*
 * Handle HTTP POST data from the client.
 */
int
do_http_post(int sd, char *req, int rlen)
{
    if (is_cmd_led(req)) {
     //   http_post_led(sd, req, rlen);
    } else if (is_cmd_switch(req)) {
     //   http_post_switch(sd, req, rlen);
    } else if (is_cmd_cdma(req)) {
        http_post_cdma(sd, req, rlen);
    } else if (is_cmd_hltp(req)) {
        http_post_hltp(sd, req, rlen);
    } else if (is_cmd_atg0(req)) {
        http_post_atg0(sd, req, rlen);
    } else if (is_cmd_atg1(req)) {
        http_post_atg1(sd, req, rlen);
    } else if (is_cmd_atg2(req)) {
        http_post_atg2(sd, req, rlen);
    } else if (is_cmd_atg3(req)) {
        http_post_atg3(sd, req, rlen);
    } else if (is_cmd_hlt0(req)) {
        http_post_hlt0(sd, req, rlen);
    } else if (is_cmd_ads0(req)) {
        http_post_ads0(sd, req, rlen);
    } else if (is_cmd_ads1(req)) {
        http_post_ads1(sd, req, rlen);
    } else if (is_cmd_ads2(req)) {
        http_post_ads2(sd, req, rlen);
    } else if (is_cmd_ads3(req)) {
        http_post_ads3(sd, req, rlen);
    } else if (is_cmd_ads4(req)) {
        http_post_ads4(sd, req, rlen);
    } else {
        xil_printf("http POST: %s unsupported command\n\r", req);
    }

    return 0;
}

/* respond for a file GET request */
int
do_http_get(int sd, char *req, int rlen)
{
    char filename[MAX_FILENAME];
    char buf[GET_BUFSIZE];
    int fsize, hlen, n;
    int fd;
    char *fext;

    /* determine file name */
    extract_file_name(filename, req, rlen, MAX_FILENAME);

    /* respond with 404 if not present */
    mfs_lock();
    n = mfs_exists_file(filename);
    mfs_unlock();
    if (n == 0) {
        xil_printf("requested file %s not found, returning 404\n\r", filename);
        do_404(sd, req, rlen);
        return -1;
    }

    /* respond with correct file */

    /* debug statement on UART */
    // xil_printf("http GET: %s\n\r", filename);

    /* get a pointer to file extension */
    fext = get_file_extension(filename);

    mfs_lock();
    fd = mfs_file_open(filename, MFS_MODE_READ);

    /* obtain file size,
     * note that lseek with offset 0, MFS_SEEK_END does not move file pointer */
    fsize = mfs_file_lseek(fd, 0, MFS_SEEK_END);
    mfs_unlock();

    /* write the http headers */
    hlen = generate_http_header(buf, fext, fsize);
    if (lwip_write(sd, buf, hlen) != hlen) {
        xil_printf("error writing http header to socket-1\n\r");
        xil_printf("http header = %s\n\r", buf);
        return -1;
    }

    /* now write the file */
    while (fsize) {
        int w;

        mfs_lock();
        n = mfs_file_read(fd, buf, BUFSIZE);
        mfs_unlock();

        if ((w = lwip_write(sd, buf, n)) != n) {
            xil_printf
                ("error writing file (%s) to socket, remaining unwritten bytes = %d\n\r",
                 filename, fsize - n);
            xil_printf
                ("attempted to lwip_write %d bytes, actual bytes written = %d\n\r",
                 n, w);
            break;
        }

        fsize -= n;
    }

    mfs_lock();
    mfs_file_close(fd);
    mfs_unlock();

    return 0;
}

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
int
generate_response(int sd, char *http_req, int http_req_len)
{
    enum http_req_type request_type =
        decode_http_request(http_req, http_req_len);

    switch (request_type) {
    case HTTP_GET:
        return do_http_get(sd, http_req, http_req_len);
    case HTTP_POST:
        return do_http_post(sd, http_req, http_req_len);
    default:
        return do_404(sd, http_req, http_req_len);
    }
}

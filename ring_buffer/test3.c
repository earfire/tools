#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ring_buffer.h"


#define FBQ_NODE_NUM 2048
#define FBQ_BUF_SZ 8

char *f_queue;
char *b_queue;

typedef struct point_s {
    int x;
    int y;
}point_t;

typedef struct fbq_node_s {
    int len;
    char buf[FBQ_BUF_SZ];
}fbq_node_t, *p_fbq_node_t;

int point_fbq_init()
{
    int i;
    fbq_node_t *node_array;

    node_array = calloc(FBQ_NODE_NUM, sizeof(point_t));
    if (!node_array) {
        fprintf(stderr, "calloc error\n");
        return -1;
    }

    rb_create(FBQ_NODE_NUM, sizeof(p_fbq_node_t), f_queue);
    rb_create(FBQ_NODE_NUM, sizeof(p_fbq_node_t), b_queue);

    for (i = 0; i < FBQ_NODE_NUM; i++) {
        rb_writeInValue(f_queue, p_fbq_node_t, &node_array[i]);
    }

    return 0;
}

int point_add(point_t *pt)
{
    fbq_node_t *buf_addr;

    if (rb_readOutValue(f_queue, p_fbq_node_t, buf_addr) != 0) {
        memcpy(buf_addr->buf, pt, sizeof(point_t));
        buf_addr->len = sizeof(point_t);

        rb_writeInValue(b_queue, p_fbq_node_t, buf_addr);
        return 0;
    }
    return -1;
}

void *point_print(void *arg)
{
    (void)arg;
    fbq_node_t *buf_addr;
    point_t *pt;

    while (1) {
        while (rb_readOutValue(b_queue, p_fbq_node_t, buf_addr) == 0) {
            usleep(1000);
            continue;
        }
        pt = (point_t *)buf_addr->buf;
        printf("point: x = %d, y = %d\n", pt->x, pt->y);
        while (rb_writeInValue(f_queue, p_fbq_node_t, buf_addr) == 0)
            ;
    }
}

int main()
{
    int i;
    int ret;
    pthread_t tid;
    point_t p = {1, 1};

    ret = point_fbq_init();
    if (ret != 0) {
        fprintf(stderr, "point_fbq_init error\n");
        exit(1);
    }
        
    if (pthread_create(&tid, NULL, point_print, NULL) != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit (1);
    }
    if (pthread_detach(tid) != 0) {
        fprintf(stderr, "pthread_detach error\n");
        exit (1);
    }


    while (1) {
        ret = point_add(&p);
        while (ret != 0) {
            usleep(10000);
            ret = point_add(&p);
        }
        p.y++;
        sleep(1);
    }
}

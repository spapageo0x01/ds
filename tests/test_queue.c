#include "../2_queue/queue.h"

int main(void)
{
        int ret;
        queue_t queue;
        queue_init(&queue);
        assert(queue);

        queue_enqueue(&queue, 10);
        queue_enqueue(&queue, 5);
        queue_enqueue(&queue, 12);

        queue_dequeue(&queue, &ret);
        printf("%d\n", ret);
        queue_dequeue(&queue, &ret);
        printf("%d\n", ret);
        queue_dequeue(&queue, &ret);
        printf("%d\n", ret);
}


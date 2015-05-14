/*************************************************************************
	> File Name: semThread.c
	> Author: henjies
	> Mail: heshengjie2011@163.com 
	> Created Time: Thu 14 May 2015 09:32:14 AM CST
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

void* producer_f(void *arg);       /*创建了两个线程，生产者和消费者*/
void* consumer_f(void *arg);
sem_t sem_producer;                         /*信号量，用于线程通信*/
sem_t sem_consumer;
int running = 1;                   /*函数运行条件*/

int main()
{
	pthread_t produer_t;
	pthread_t consumer_t;

	sem_init(&sem_producer,0,1);           /*参数2表示只能在当前进程不同线程中使用，若不为0则可以在不同进程中使用，参数3表示初始信号量大小是16*/
	sem_init(&sem_consumer,0,0);

	pthread_create(&produer_t,NULL,(void*)producer_f,NULL);
	pthread_create(&consumer_t,NULL,(void*)consumer_f,NULL);

	usleep(100000);
	running = 0;
	pthread_join(consumer_t,NULL);/*等待线程结束*/
	pthread_join(produer_t,NULL);

	sem_destroy(&sem_producer);/*销毁信号量*/
	sem_destroy(&sem_consumer);

	return 0;
}

/* 生产者线程函数*/
void* producer_f(void *arg)
{
	int semval = 0;
	while(running)
	{
		usleep(1);
		sem_wait(&sem_producer);
		/*sem_post(&sem);*/
		sem_getvalue(&sem_producer,&semval);
		printf("生产者，总数量：%d\n",semval);
		sem_post(&sem_consumer);
	}

	return NULL;
}

/*消费者线程函数 */
void* consumer_f(void *arg)
{
	int semval = 0;
	while(running)
	{
		usleep(1);
		sem_wait(&sem_consumer);
		sem_getvalue(&sem_consumer,&semval);
		printf("消费者，总数量：%d\n",semval);
		sem_post(&sem_producer);
	}

	return NULL;
}

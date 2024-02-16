#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/*
test destroy
*/
void test_destroy_not_empty(void)
{
	fprintf(stderr, "*** TEST test_destroy_nOT_EMPTY ***\n");
    
    int data = 3;
    queue_t q;
    q = queue_create();
	queue_enqueue(q, &data);

	TEST_ASSERT(queue_destroy(q) == -1);

}
void test_destroy_null(void)
{
	fprintf(stderr, "*** TEST test_destroy_null ***\n");
    queue_t q;
	q = queue_create();
    
    queue_destroy(q);
	TEST_ASSERT(queue_destroy(NULL) == -1);
}
void test_destroy_regular_one(void)
{
	fprintf(stderr, "*** TEST test_destroy_regular_one ***\n");
	queue_t q;

	q = queue_create();
	
	TEST_ASSERT(queue_destroy(q) == 0);

}
void test_destroy_regular_two(void)
{
	fprintf(stderr, "*** TEST test_destroy_regular_two ***\n");

    int data = 3, *ptr;
	queue_t q;

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);

	TEST_ASSERT(queue_destroy(q) == 0);

}
/*
test enqueue
*/
void enqueue_test_one()
{
	fprintf(stderr, "*** TEST enqueue_test_one ***\n");

    queue_t q;

	q = queue_create();


    short int a = 2;
    char *b = "P2 will teach me threads";
    struct {
    int stuff;
    char thing;
    } c = { 10, 'c' };
    int *d = malloc(10 * sizeof(int));
    TEST_ASSERT(queue_length(q)==0);
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);
    queue_enqueue(q, b);
    TEST_ASSERT(queue_length(q)==2);
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);
    queue_enqueue(q, d);
    TEST_ASSERT(queue_length(q)==4);


}

void enqueue_test_data_null()
{
	fprintf(stderr, "*** TEST enqueue_test_data_null ***\n");

    queue_t q;

	q = queue_create();

    
    TEST_ASSERT(queue_enqueue(q, NULL)==-1);


}
void enqueue_test_queue_null()
{
	fprintf(stderr, "*** TEST enqueue_test_queue_null ***\n");


    queue_t q;
	q = queue_create();

    queue_destroy(q);

    int a = 3;

    
    TEST_ASSERT(queue_enqueue(NULL, &a)==-1);


}

/*
test dequeue
*/
void dequeue_test_one()
{
	fprintf(stderr, "*** TEST dequeue_test_one ***\n");

    queue_t q;

	q = queue_create();


    short int a = 2;
    char *b = "P2 will teach me threads";
    struct {
    int stuff;
    char thing;
    } c = { 10, 'c' };
    int *d = malloc(10 * sizeof(int));
    TEST_ASSERT(queue_length(q)==0);
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);
    queue_enqueue(q, b);
    TEST_ASSERT(queue_length(q)==2);
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);
    queue_enqueue(q, d);
    TEST_ASSERT(queue_length(q)==4);


    short int *e;
    char *f;

    struct tes{
    int stuff;
    char thing;
    };
    struct tes *g;;


    int *h;

    queue_dequeue(q, (void**)&e);
    TEST_ASSERT(queue_length(q)==3);
    TEST_ASSERT(*e==2);
    
    queue_dequeue(q, (void**)&f);
    TEST_ASSERT(queue_length(q)==2);
    TEST_ASSERT(!strcmp(f,"P2 will teach me threads"));


    queue_dequeue(q, (void**)&g);
    TEST_ASSERT(queue_length(q)==1);
    TEST_ASSERT(g->stuff==10);
    TEST_ASSERT(g->thing=='c');


    queue_dequeue(q, (void**)&h);
    TEST_ASSERT(queue_length(q)==0);
    TEST_ASSERT(sizeof(*h)==(sizeof(int)));
}

void dequeue_test_null_queue()
{
	fprintf(stderr, "*** TEST dequeue_test_one ***\n");

    int a;
    TEST_ASSERT(queue_dequeue(NULL, (void**)&a)==-1);

}
void dequeue_test_null_data()
{
	fprintf(stderr, "*** TEST dequeue_test_one ***\n");
    queue_t q;

	q = queue_create();
    int a;
    TEST_ASSERT(queue_dequeue(q, (void**)&a)==-1);

}
void dequeue_test_empty_queue()
{
	fprintf(stderr, "*** TEST dequeue_test_one ***\n");

    TEST_ASSERT(queue_dequeue(NULL, (void**)NULL)==-1);

}
/*
Enqueue / Dequeue

*/
void enqueue_dequeue()
{
	fprintf(stderr, "*** TEST delete_test_one ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);

    int *e;
    queue_dequeue(q, (void **)&e);
    TEST_ASSERT(queue_length(q)==3);
    TEST_ASSERT(*e==1);

    int *f;
    queue_dequeue(q, (void **)&f);
    TEST_ASSERT(queue_length(q)==2);   
    TEST_ASSERT(*f==2); 

    int *g;
    queue_dequeue(q, (void **)&g);
    TEST_ASSERT(queue_length(q)==1);   
    TEST_ASSERT(*g==3);  

    int *h;
    queue_dequeue(q, (void **)&h);
    TEST_ASSERT(queue_length(q)==0);  
    TEST_ASSERT(*h==4);        


}
/*
delete
*/
void delete_test_one()
{
	fprintf(stderr, "*** TEST delete_test_one ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);




    //int test = 4;
    queue_delete(q, &d);
    TEST_ASSERT(queue_length(q)==3);



    int *e;
    queue_dequeue(q, (void **)&e);
    TEST_ASSERT(queue_length(q)==2);
    TEST_ASSERT(*e==1);

    int *f;
    queue_dequeue(q, (void **)&f);
    TEST_ASSERT(queue_length(q)==1);   
    TEST_ASSERT(*f==2); 

    int *g;
    queue_dequeue(q, (void **)&g);
    TEST_ASSERT(queue_length(q)==0);   
    TEST_ASSERT(*g==3);  
       


}

void delete_test_two()
{
	fprintf(stderr, "*** TEST delete_test_two ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);




    //int test = 3;
    queue_delete(q, &c);
    TEST_ASSERT(queue_length(q)==3);



    int *e;
    queue_dequeue(q, (void **)&e);
    TEST_ASSERT(queue_length(q)==2);
    TEST_ASSERT(*e==1);

    int *f;
    queue_dequeue(q, (void **)&f);
    TEST_ASSERT(queue_length(q)==1);   
    TEST_ASSERT(*f==2); 

    int *g;
    queue_dequeue(q, (void **)&g);
    TEST_ASSERT(queue_length(q)==0);   
    TEST_ASSERT(*g==4);  
       


}


void delete_test_three()
{
	fprintf(stderr, "*** TEST delete_test_three ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);




    //int test = 2;
    queue_delete(q, &b);
    TEST_ASSERT(queue_length(q)==3);



    int *e;
    queue_dequeue(q, (void **)&e);
    TEST_ASSERT(queue_length(q)==2);
    TEST_ASSERT(*e==1);

    int *f;
    queue_dequeue(q, (void **)&f);
    TEST_ASSERT(queue_length(q)==1);   
    TEST_ASSERT(*f==3); 

    int *g;
    queue_dequeue(q, (void **)&g);
    TEST_ASSERT(queue_length(q)==0);   
    TEST_ASSERT(*g==4);  
       


}



void delete_test_four()
{
	fprintf(stderr, "*** TEST delete_test_four ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);




    //int test = 1;
    queue_delete(q, &a);
    TEST_ASSERT(queue_length(q)==3);



    int *e;
    queue_dequeue(q, (void **)&e);
    TEST_ASSERT(queue_length(q)==2);
    TEST_ASSERT(*e==2);

    int *f;
    queue_dequeue(q, (void **)&f);
    TEST_ASSERT(queue_length(q)==1);   
    TEST_ASSERT(*f==3); 

    int *g;
    queue_dequeue(q, (void **)&g);
    TEST_ASSERT(queue_length(q)==0);   
    TEST_ASSERT(*g==4);  
       


}

void delete_test_no_queue()
{
	fprintf(stderr, "*** TEST delete_test_no_queue ***\n");
    int a = 10;
    TEST_ASSERT(queue_delete(NULL, (void**)&a)==-1);
}

void delete_test_no_data()
{
	fprintf(stderr, "*** TEST delete_test_no_data ***\n");
    queue_t q;

	q = queue_create();
    TEST_ASSERT(queue_delete(q, NULL)==-1);
}
void delete_test_not_found()
{
	fprintf(stderr, "*** TEST delete_test_not_found ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);




    int test = 10;
    TEST_ASSERT(queue_delete(q, &test)==-1);
    

}

static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_iterator_one()
{
    fprintf(stderr, "*** TEST test_iterator_one ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    queue_iterate(q, iterator_inc);
    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);
}


void test_iterator_no_queue()
{
    fprintf(stderr, "*** TEST test_iterator_no_queue ***\n");

    /* Increment every item of the queue, delete item '42' */
    TEST_ASSERT(queue_iterate(NULL, iterator_inc)==-1);
}

void test_iterator_no_func()
{
    fprintf(stderr, "*** TEST test_iterator_no_func ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    TEST_ASSERT(queue_iterate(q, NULL)==-1);
}
/*
length
*/

void test_length()
{
	fprintf(stderr, "*** TEST test_length ***\n");

    queue_t q;

	q = queue_create();

    /* Push in queue */
    TEST_ASSERT(queue_length(q)==0);

    int a=1;
    queue_enqueue(q, &a);
    TEST_ASSERT(queue_length(q)==1);

    int b=2;
    queue_enqueue(q, &b);
    TEST_ASSERT(queue_length(q)==2);

    int c=3;
    queue_enqueue(q, &c);
    TEST_ASSERT(queue_length(q)==3);

    int d=4;
    queue_enqueue(q, &d);
    TEST_ASSERT(queue_length(q)==4);






}
void test_length_null_queue()
{
	fprintf(stderr, "*** TEST test_length_null_queue ***\n");

    TEST_ASSERT(queue_length(NULL)==-1);
}
int main(void)
{
    //DESTROY
    test_destroy_null();
    test_destroy_not_empty();
    test_destroy_regular_one();
    test_destroy_regular_two();

    //ENQUEUE
    enqueue_test_one();
    enqueue_test_data_null();
    enqueue_test_queue_null();


    //DEQUEUE
    dequeue_test_one();
    enqueue_dequeue();
    dequeue_test_null_queue();
    dequeue_test_null_data();
    dequeue_test_empty_queue();

    //DELETE
    delete_test_one();
    delete_test_two();
    delete_test_three();
    delete_test_four();
    delete_test_no_queue();
    delete_test_no_data();
    delete_test_not_found();

    //ITERATE
    test_iterator_one();
    test_iterator_no_queue();
    test_iterator_no_func();

    //LENGTH
    test_length();
    test_length_null_queue();


	return 0;
}

# include  <stdio.h>
# include  <stdlib.h>
# include  <pthread.h>
# include  <unistd.h>
# include  <semaphore.h>
#include <iostream>
#include <time.h>
#include "cater.h"
using namespace std ;

# define  TRUE  1
# define  FALSE  0
# define  MAX  3


pthread_t caterers [ 3 ];


pthread_t Agent, Agent_A, Agent_B, Agent_C;

pthread_t Pusher_A, Pusher_B, Pusher_C;

pthread_mutex_t pushers, Smoke;


int product [ 3 ];
int MAX_HOTDOGS ;



int hotdogs_done_so_far = 0 , hotdogs [ 3 ] = { 0 , 0 , 0 };


sem_t sausage, bread, mustard, SemAgent, SemBread, SemSausage, SemMustard;





 Cater :: Cater (int id) {
    pthread_mutex_lock (& Smoke);
    if (hotdogs_done_so_far>= MAX_HOTDOGS) {
        cout << " MAX HOTDOGS TO BE PREPARED !!! " ;
        pthread_mutex_unlock (& Smoke);
        return ;
    }
    if (id == 1 ) {
        hotdogs [ 0 ] ++;
    } else  if (id == 2 ) {
        hotdogs [ 1 ] ++;
    } else {
        hotdogs [ 2 ] ++;
    }
    hotdogs_done_so_far ++;
    pthread_mutex_unlock (& Smoke);
    switch (id) {

        case  1 :
            cout << " Caterer with Bread STARTED preparing ... (as Supplier provides Sausages and Mustard) "<< endl ;

            break ;
        case  2 :
            cout << " Caterer with Sausages STARTED preparing ... (as Supplier provides Mustard and Bread)"<< endl;

            break ;
        case  3 :
            cout << " Caterer with Mustard STARTED preparing ... (as Supplier provides Sausages and Bread) " << endl ;

            break ;
    }
   sleep ( 1 );
    pthread_mutex_lock (& Smoke);
    switch (id) {

        case  1 :
            cout << " Caterer with Bread has FINISHED preparing ... " << endl;
            cout << endl ;
            break ;
        case  2 :
             cout <<  " Caterer with Sausages has FINISHED preparing ... "<< endl ;
             cout << endl;
            break ;
        case  3 :
            cout <<" Caterer with Mustard FINISHED preparing ...  " << endl;
            cout << endl;
            break ;
    }
    cout <<  " Hot-Dogs Prepared So far ...  " << endl;
    cout << " w/Bread :" <<hotdogs [ 0 ]<< " w/Sausages :" <<hotdogs [ 1 ]<< " w/Mustard :" <<hotdogs [ 2 ]<< endl;
    cout << endl ;
    pthread_mutex_unlock (& Smoke);
}


void * pusher_A ( void * in) {
    while ( 1 ) {
        sem_wait (& bread);
        pthread_mutex_lock (& pushers);
        if (product [ 0 ] == MAX) {

            if (product [ 1 ]) {
                product [ 0 ] --;
                product [ 1 ] --;
                sem_post (& SemMustard);
            } else  if (product [ 2 ]) {
                product [ 2 ] --;
                product [ 0 ] --;
                sem_post (& SemSausage);
            }
        } else {

            if (product [ 1 ]) {
                product [ 1 ] --;
                sem_post (& SemMustard);
            } else  if (product [ 2 ]) {
                product [ 2 ] --;
                sem_post (& SemSausage);
            } else  if (product [ 0 ] <MAX) {
                product [ 0 ] ++;
            }
        }
        pthread_mutex_unlock (& pushers);
    }
}

void * pusher_B ( void * in) {

    while ( 1 ) {
        sem_wait (& sausage);
        pthread_mutex_lock (& pushers);
        if (product [ 1 ] == MAX) {

            if (product [ 0 ]) {
                product [ 1 ] --;
                product [ 0 ] --;
                sem_post (& SemMustard);
            } else  if (product [ 2 ]) {
                product [ 1 ] --;
                product [ 2 ] --;
                sem_post (& SemBread);
            }

        } else {

            if (product [ 0 ]) {
                product [ 0 ] --;
                sem_post (& SemMustard);
            } else  if (product [ 2 ]) {
                product [ 2 ] --;
                sem_post (& SemBread);
            } else  if (product [ 1 ] <MAX) {
                product [ 1 ] ++;
            }
        }
        pthread_mutex_unlock (& pushers);
    }
}

void * pusher_C ( void * in) {

    while ( 1 ) {
        sem_wait (& mustard);
        pthread_mutex_lock (& pushers);
        if (product [ 2 ] == MAX) {

            if (product [ 0 ]) {
                product [ 2 ] --;
                product [ 0 ] --;
                sem_post (& SemSausage);
            } else  if (product [ 1 ]) {
                product [ 2 ] --;
                product [ 1 ] --;
                sem_post (& SemBread);
            }
        } else {

            if (product [ 0 ]) {
                product [ 0 ] --;
                sem_post (& SemSausage);
            } else  if (product [ 1 ]) {
                product [ 1 ] --;
                sem_post (& SemBread);
            } else  if (product [ 2 ] < 3 ) {
                product [ 2 ] ++;
            }
        }
        pthread_mutex_unlock (& pushers);
    }
}


void * agent ( void * in) {
    while ( 1 ) {
        sleep ( 1 );
        sem_post (& SemAgent);
    }
}

void * agent_A ( void * in) {
    while ( 1 ) {
        sem_wait (& SemAgent);
        sem_post (& sausage);
        sem_post (& mustard);
    }
}

void * agent_B ( void * in) {
    while ( 1 ) {
        sem_wait (& SemAgent);
        sem_post (& bread);
        sem_post (& mustard);
    }
}

void * agent_C ( void * in) {
    while ( 1 ) {
        sem_wait (& SemAgent);
        sem_post (& bread);
        sem_post (& sausage);
    }
}

void * smoker_bread ( void * in) {
    while ( 1 ) {
        sem_wait (& SemBread);
        pthread_mutex_lock (& Smoke);
        if (hotdogs_done_so_far>= MAX_HOTDOGS) {
            pthread_mutex_unlock (& Smoke);
            pthread_exit ( NULL );
        } else {

            cout << " Caterer with Bread is READY to make ...  "<< endl ;
            pthread_mutex_unlock (& Smoke);
            Cater ( 1 );
        }
    }
}

void * smoker_sausage ( void * in) {
    while ( 1 ) {
        sem_wait (& SemSausage);
        pthread_mutex_lock (& Smoke);
        if (hotdogs_done_so_far>= MAX_HOTDOGS) {
            pthread_mutex_unlock (& Smoke);
            pthread_exit ( NULL );
        } else {

            cout << " Caterer with Sausage is READY to make ... " << endl;
            pthread_mutex_unlock (& Smoke);
            Cater ( 2 );
        }
    }
}

void * smoker_mustard ( void * in) {
    while ( 1 ) {
        sem_wait (& SemMustard);
        pthread_mutex_lock (& Smoke);
        if (hotdogs_done_so_far >= MAX_HOTDOGS) {
            pthread_mutex_unlock (& Smoke);
            pthread_exit ( NULL );
        } else {
            // else the smoker smokes
            cout << " Smoker with Mustard is READY to make ... " << endl;
            pthread_mutex_unlock (& Smoke);
            Cater ( 3 );
        }
    }
}

int Cater :: num_hot (){
int n ;
cout<< endl;
cout << "Enter the number of Hot-Dogs to be prepared between the participants : "  ;
cin >> n ;
return n;
}

int  main () {
    cout << "-------" << "HOT-DOG PROBLEM"<< "--------" << endl ;
    cout << endl ;
    cout << " The problem revolves around the following participants : " << endl ;
    cout << " A Supplier " << endl ;

    cout << " Caterer with Bread " << endl;

    cout << " Caterer with Sausages " << endl;

    cout << " Caterer with Mustard " << endl;
    cout << endl;
    cout << endl ;

    Cater c(0);
    MAX_HOTDOGS = c.num_hot() ;

    srand ( time ( NULL ));
    for ( int i = 0 ; i < 2 ; i ++) {
        product [i] = FALSE ;
    }

    sem_init (& sausage, 1 , 0 );
    sem_init (& bread, 1 , 0 );
    sem_init (& mustard, 1 , 0 );
    sem_init (& SemAgent, 1 , 0 );
    sem_init (& SemBread, 1 , 0 );
    sem_init (& SemSausage, 1 , 0 );
    sem_init (& SemMustard, 1 , 0 );

    pthread_create (& Agent, NULL , & agent, NULL );
    pthread_create (& caterers [ 0 ], NULL , & smoker_bread, NULL );
    pthread_create (& caterers [ 1 ], NULL , & smoker_sausage, NULL );
    pthread_create (& caterers [ 2 ], NULL , & smoker_mustard, NULL );
    pthread_create (& Agent_A, NULL , & agent_A, NULL );
    pthread_create (& Agent_B, NULL , & agent_B, NULL );
    pthread_create (& Agent_C, NULL , & agent_C, NULL );
    pthread_create (& Pusher_A, NULL , & pusher_A, NULL );
    pthread_create (& Pusher_B, NULL , & pusher_B, NULL );
    pthread_create (& Pusher_C, NULL , & pusher_C, NULL );

    pthread_join (caterers [ 0 ], NULL );
    pthread_join (caterers [ 1 ], NULL );
    pthread_join (caterers [ 2 ], NULL );

    cout << endl ;
    cout << endl ;
    cout << endl ;
    cout << endl ;
    cout << " Caterer with Bread has prepared " << hotdogs [ 0 ] << " hot-dogs " << endl ;
    cout << endl ;
    cout << " Caterer with Sausages has prepared "<< hotdogs [ 1 ] << " hot-dogs " << endl ;
    cout << endl;
    cout << " Caterer with Mustard has prepared " << hotdogs [ 2 ]<< " hot-dogs " << endl ;
    cout << endl ;
    cout << "Maximum Hot-Dogs that have been prepared : " << hotdogs_done_so_far << endl ;
    return  0 ;
}




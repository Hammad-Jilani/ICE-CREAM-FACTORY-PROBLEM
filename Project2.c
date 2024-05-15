#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/select.h>

#define NUM_FLAVOR 3
#define MAX_CAPACITY 100
#define MAX_TEMP 30
#define MIN_TEMP 20
#define CLEANING_TIME 5
#define MAX_FREEZER_CAPACITY 200

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} MySem;

void work();
void work2();
void *Production_Handler (void *arg);
void *Temperature_Handler (void *arg);
void *Cleaning_Handler (void *arg);
void *Demand_Handler(void *arg);
void SaveData();
void SaveData2b();
void SaveData2a();
int find_max();
bool readData();
void printData();
void ReadData2();
int my_sem_init(MySem *sem,int i);
int my_sem_wait(MySem *sem);
int my_sem_signal(MySem *sem);

MySem temp_mutex;
MySem freezer_mutex;
MySem clean_mutex;
int w = 0;

typedef struct FlavQuant {
	char *var;
	int quantity;
	int maxQuant;
	int produced;
	int demand;
}FlavQuant;
int temperature = 25, freezer_space = MAX_FREEZER_CAPACITY;
bool check_temp = true, order_complete = false, max_complete = false, end = false;
FlavQuant FQ[3];
//int temp_now = 19;
void start(){
	printf("\n\n\n\t\t\t$$$$$$\\  $$$$$$\\  $$$$$$$$\\              $$$$$$\\  $$$$$$$\\  $$$$$$$$\\  $$$$$$\\  $$\\      $$\\ \n");
	printf("\t\t\t\\_$$  _|$$  __$$\\ $$  _____|            $$  __$$\\ $$  __$$\\ $$  _____|$$  __$$\\ $$$\\    $$$ |\n");
	printf("\t\t\t  $$ |  $$ /  \\__|$$ |                  $$ /  \\__|$$ |  $$ |$$ |      $$ /  $$ |$$$$\\  $$$$ |\n");
	printf("\t\t\t  $$ |  $$ |      $$$$$\\                $$ |      $$$$$$$  |$$$$$\\    $$$$$$$$ |$$\\$$\\$$ $$ |\n");
	printf("\t\t\t  $$ |  $$ |      $$  __|               $$ |      $$  __$$< $$  __|   $$  __$$ |$$ \\$$$  $$ |\n");
	printf("\t\t\t  $$ |  $$ |  $$\\ $$ |                  $$ |  $$\\ $$ |  $$ |$$ |      $$ |  $$ |$$ |\\$  /$$ |\n");
	printf("\t\t\t$$$$$$\\ \\$$$$$$  |$$$$$$$$\\             \\$$$$$$  |$$ |  $$ |$$$$$$$$\\ $$ |  $$ |$$ | \\_/ $$ |\n");
	printf("\t\t\t\\______| \\______/ \\________|             \\______/ \\__|  \\__|\\________|\\__|  \\__|\\__|     \\__|\n");
	printf("\t\t\t                                                                                             \n");
	printf("\t\t\t                                                                                             \n");
	printf("\t\t\t                                                                                             \n");
	printf("\t\t\t            $$$$$$$$\\  $$$$$$\\   $$$$$$\\ $$$$$$$$\\  $$$$$$\\  $$$$$$$\\ $$\\     $$\\            \n");
	printf("\t\t\t            $$  _____|$$  __$$\\ $$  __$$\\\\__$$  __|$$  __$$\\ $$  __$$\\\\$$\\   $$  |           \n");
	printf("\t\t\t            $$ |      $$ /  $$ |$$ /  \\__|  $$ |   $$ /  $$ |$$ |  $$ |\\$$\\ $$  /            \n");
	printf("\t\t\t            $$$$$\\    $$$$$$$$ |$$ |        $$ |   $$ |  $$ |$$$$$$$  | \\$$$$  /             \n");
	printf("\t\t\t            $$  __|   $$  __$$ |$$ |        $$ |   $$ |  $$ |$$  __$$<   \\$$  /              \n");
	printf("\t\t\t            $$ |      $$ |  $$ |$$ |  $$\\   $$ |   $$ |  $$ |$$ |  $$ |   $$ |               \n");
	printf("\t\t\t            $$ |      $$ |  $$ |\\$$$$$$  |  $$ |    $$$$$$  |$$ |  $$ |   $$ |               \n");
	printf("\t\t\t            \\__|      \\__|  \\__| \\______/   \\__|    \\______/ \\__|  \\__|   \\__|               \n");
}
int main() {
	srand(time(NULL));
	system("clear");
	start();
	sleep(3);
	system("clear");
	my_sem_init(&temp_mutex, w);
	my_sem_init(&freezer_mutex, w);
	my_sem_init(&clean_mutex, w);
	work();
	work2();
	exit(0);
}
void work() {
	printf("\n\n\n");
	printf("\t\t\t.-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-.\n");
	printf("\t\t\t|                                                                       |\n");
	printf("\t\t\t|                      PLEASE CHOOSE THE FLAVOUR                        |\n");
	printf("\t\t\t!                                                                       !\n");
	printf("\t\t\t:          	 	                                                :\n");
	printf("\t\t\t:                           1. VANILLA                                  :\n");
	printf("\t\t\t.                                                                       .\n");
	printf("\t\t\t.                                                                       .\n");
	printf("\t\t\t:                           2. CHOCOLATE                                :\n");
	printf("\t\t\t:                 	                                                :\n");
	printf("\t\t\t!                                                                       !\n");
	printf("\t\t\t|                           3. STRAWBERRY                               |\n");
	printf("\t\t\t|                                                                       |\n");
	printf("\t\t\t`-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-'\n");
}
void work2() {
	FQ[0].var = "VANILLA";
	FQ[1].var = "CHOCOLATE";
	FQ[2].var = "STRAWBERRY";
	char e = 'a';
	if(!readData()) {
		printf("\n\t\t\tPrevious Order was Not Complete.\n\t\t\tDo you want to continue with previous Order? Press Y\n\t\t\t");
		ReadData2();
		e = getchar();
		for(int i = 0; i < NUM_FLAVOR; i++) {
			FQ[i].maxQuant -= MAX_CAPACITY;
		}
	}
	FILE *fptr;
	char string[10];
	fptr = fopen("flavour.txt","r");
	if(fptr == NULL) {
		for(int i = 0; i < 3; i++) {
			//FQ[i].quantity = 0;
			FQ[i].produced = 0;
			FQ[i].demand = 0;
			FQ[i].maxQuant = MAX_CAPACITY;
		}
	}
	else{
		for(int i = 0; i < 3; i++) {
			fgets(string,10,fptr);
			//FQ[i].quantity = 0;
			FQ[i].maxQuant = atoi(string);
			FQ[i].maxQuant += MAX_CAPACITY;
		}
	}
	fclose(fptr);
	while(1) {
		if(e != 'Y' && e != 'y') {
			printf("\n\n\t\t\tIf not selected, Enter 0 for the relevant flavour.\n");
			int quantity;
			printf("\t\t\t");
			scanf("%d",&quantity);
			FQ[0].quantity = quantity;
			printf("\t\t\t");
			scanf("%d",&quantity);
			FQ[1].quantity = quantity;
			printf("\t\t\t");
			scanf("%d",&quantity);
			FQ[2].quantity = quantity;
		}
		e = 'a';
		SaveData2a();
		check_temp = true;
		order_complete = false;
		max_complete = false;
		pthread_t temp;
		pthread_create(&temp, NULL, Temperature_Handler, NULL);
		pthread_t product[3], clean, demand;
		pthread_create(&demand, NULL, Demand_Handler, NULL);
		pthread_create(&clean, NULL, Cleaning_Handler, NULL);
		for(int j = 0; j < NUM_FLAVOR; j++) {
			int *flav = malloc(sizeof(int));
        	*flav = j;
			pthread_create(&product[j], NULL, Production_Handler, (void *)flav);
		}
		for (int i = 0; i < NUM_FLAVOR; i++) {
    		pthread_join(product[i], NULL);
    	}
		sleep(3);
		order_complete = true;
		printf("\n\n\t\t\tOrder Completed Successfuly.\n");
		SaveData2b();
		my_sem_signal(&clean_mutex);
		pthread_join(temp, NULL);
		pthread_join(demand,NULL);
		pthread_join(clean,NULL);
		SaveData();
		sleep(3);	
		system("clear");
		work();
		FQ[0].quantity = 0;
		FQ[1].quantity = 0;
		FQ[2].quantity = 0;
		printf("\n\n\t\t\tDo You Want to End the Day?Press 'Q'.\n\t\t\t");
		char d;
		scanf("%c", &d);
		char c = getchar();
		if(c == 'q' || c == 'Q') {
			end = true;
			sleep(2);
			break;
		}
		if(freezer_space <=0) {
			printf("\n\t\t\tDAILY PRODUCTION REACHED>\n\t\t\tDAY IS CLOSING.\n");
			sleep(2);
			break;
		}
	}
	printData();
}
void *Demand_Handler(void *arg) {
	while (1)
	{
		if(order_complete) {
				break;
		}
		for (int i = 0; i < NUM_FLAVOR; i++)
		{
			FQ[i].demand = FQ[i].produced / 10;
		}
		sleep(5);
	}
}
void *Temperature_Handler(void *arg) {
	while (1) {
        	if(order_complete) {
				break;
			}
			if(check_temp) {
        		temperature = rand() % (MAX_TEMP - MIN_TEMP + 1) + MIN_TEMP;
        		printf("\n\n\t\t\tCurrent temperature: %d°C\n", temperature);
				check_temp = false;
        		my_sem_signal(&temp_mutex);
			}
			sleep(5);
    }
    return NULL;
}
void *Cleaning_Handler(void *arg) {
	while (1) {
		if(freezer_space <= 0 || end) {
			break;
		}
		if(order_complete) {
			my_sem_wait(&clean_mutex);
    		printf("\n\n\t\t\tPerforming cleaning and sanitation...\n");
    		sleep(CLEANING_TIME);
    		printf("\t\t\tCleaning and sanitation complete.\n");
			order_complete = false;
			break;
		}
		sleep(5);
	}
    return NULL;
}
void *Production_Handler(void *arg) {
	int id = *((int *)arg);
	int temp_now = temperature;
	int max = find_max();
	while(1) {
		if(!check_temp){
			my_sem_wait(&temp_mutex);
			if(temp_now >= MIN_TEMP && temp_now <= MAX_TEMP) {
				if(max == FQ[id].demand || max_complete) {
    				my_sem_signal(&freezer_mutex);
					if(freezer_space >= 1) {
						if(FQ[id].quantity <= FQ[id].maxQuant) {
							if(id == 0){
								for(int i = 0; i < FQ[id].quantity; i++) {
									printf("\n\t\t\tVANILLA ice cream is under process by THREAD %d",id);
										FQ[id].maxQuant--;
									freezer_space--;
									FQ[id].produced++;
									sleep(2);
								}
							}
								else if(id == 1) {
								for(int i = 0; i < FQ[id].quantity; i++) {
									printf("\n\t\t\tCHOCOLATE ice cream is under process by THREAD %d",id);
									FQ[id].maxQuant--;
									freezer_space--;
									FQ[id].produced++;
									sleep(2);
								}
							}
							else if(id == 2){
								for(int i = 0; i < FQ[id].quantity; i++) {
									printf("\n\t\t\tSTRAWBERRY ice cream is under process by THREAD %d",id);
									FQ[id].maxQuant--;
									freezer_space--;
										FQ[id].produced++;
									sleep(2);
								}
							}
						}
						else {
							printf("\n\t\t\tWe are Sorry.\n\t\t\tStock is currently out of order for the %s ice cream.\n\n\t\t\tPlease Re-order Another Falvour.\n", FQ[id].var);
								sleep(4);
						}
					}
					else {
						printf("\n\t\t\tWe are Sorry.\n\t\t\tMAX production is completed for the day.\n");
						sleep(4);
					}
					max_complete = true;
					my_sem_wait(&freezer_mutex);
					my_sem_signal(&temp_mutex);
					break;
				}
				else {
					sleep(2);
				}
			}
			else{
				printf("\n\t\t\tCurrent Temperature is not Suitable.\n\t\t\tWaiting for appropriate temperature.\n");
				my_sem_signal(&temp_mutex);
				while (temp_now <= MIN_TEMP || temp_now >= MAX_TEMP)
				{
					check_temp = true;
					temp_now = temperature;
				}
				my_sem_wait(&freezer_mutex);
			}
		}
		else{
			sleep(2);
		}
	}
	return NULL;
}
void SaveData2a() {
	FILE *fptr;
	fptr = fopen("Order.txt","a");
	fprintf(fptr,"%d\n",FQ[0].quantity);
	fprintf(fptr,"%d\n",FQ[1].quantity);
	fprintf(fptr,"%d\n",FQ[2].quantity);
	fprintf(fptr,"Incomplete\n");
	fclose(fptr);
}
void SaveData2b() {
	FILE *fptr1, *fptr2;
    char line1[20],line2[20];
    fptr1 = fopen("Order.txt", "r");
    if (fptr1 == NULL) {
        return;
    }
    fptr2 = fopen("temp.txt", "w");
    if (fptr2 == NULL) {
        fclose(fptr1);
        return;
    }
	fgets(line1, sizeof line1, fptr1);
	while (fgets(line2, sizeof line2, fptr1)) {
    	fprintf(fptr2, "%s", line1);
    	strcpy(line1, line2);
	}
    fclose(fptr1);
    fclose(fptr2);
    remove("Order.txt");
    rename("temp.txt", "Order.txt");
    fptr1 = fopen("Order.txt","a");
	fprintf(fptr1,"complete\n");
	fclose(fptr1);
	return;

}
void SaveData() {
	FILE *fptr;
	fptr = fopen("flavour.txt","w");
	fprintf(fptr,"%d\n",FQ[0].maxQuant);
	fprintf(fptr,"%d\n",FQ[1].maxQuant);
	fprintf(fptr,"%d\n",FQ[2].maxQuant);
	fclose(fptr);
}
int find_max() {
	int m = FQ[0].demand;
	for (int i = 0; i < NUM_FLAVOR; i++)
	{
		if(FQ[i].demand > m) {
			m = FQ[i].demand;
		}
	}
	return m;
}
void ReadData2() {
	FILE *fptr;
	char string[20];
	fptr = fopen("Order.txt","r");
	if(fptr == NULL) {
		return;
	}
	while (fgets(string,20,fptr) != NULL) {
		FQ[0].quantity = atoi(string);
		fgets(string,20,fptr);
		FQ[1].quantity = atoi(string);
		fgets(string,20,fptr);
		FQ[2].quantity = atoi(string);
		fgets(string,20,fptr);
	}

}
bool readData() {
	FILE *fptr;
	char string[20], string2[20];
	fptr = fopen("Order.txt","r");
	if(fptr == NULL) {
		return true;
	}
	while(fgets(string,20,fptr) != NULL) {
		if (string2 != NULL) {
            //free(string2);
        }
        strncpy(string2,string,sizeof(string));
        string[0] = '\0';
	}
	if(strcmp(string2,"Incomplete\n") == 0) {
		return false;
	}
	else {
		return true;
	}
}
int my_sem_init(MySem *sem, int i){
    if(i < 0) {
            return -1;
    }
    sem->value = i;
    pthread_mutex_init(&sem->mutex,NULL);
    pthread_cond_init(&sem->cond, NULL);
}

int my_sem_wait(MySem *sem){
	pthread_mutex_lock(&sem->mutex);

        while (sem->value <= 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    	}	
	sem->value--;

    	pthread_mutex_unlock(&sem->mutex);

    	return 0;
}
int my_sem_signal(MySem *sem){
    pthread_mutex_lock(&sem->mutex);

    sem->value++;
    pthread_cond_signal(&sem->cond);

    pthread_mutex_unlock(&sem->mutex);

    return 0;
}
void printData() {
	printf("\n\n\t\t\tTotal Production Today\n");
	printf("\t\t\tVanilla: %d\n", FQ[0].produced);
	printf("\t\t\tChocolate: %d\n", FQ[1].produced);
	printf("\t\t\tStrawberry: %d\n", FQ[2].produced);
	printf("\n\t\t\tTotal Produced: %d\n", FQ[0].produced + FQ[1].produced + FQ[2].produced);
	printf("\n\n\t\t\tRemaining Items Today\n");
	printf("\t\t\tVanilla: %d\n", FQ[0].maxQuant);
	printf("\t\t\tChocolate: %d\n", FQ[1].maxQuant);
	printf("\t\t\tStrawberry: %d\n", FQ[2].maxQuant);
	printf("\n\t\t\tTotal Remaining: %d\n", FQ[0].maxQuant + FQ[1].maxQuant + FQ[2].maxQuant);
}






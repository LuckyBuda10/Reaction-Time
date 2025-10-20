#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>

//File that stores highscore
#define FILE_NAME "highscore.txt"

//Display each command along with a brief description
void print_commands();

//Give instructions on how the game works
void how_to_play();

/*
  Gives player a small countdown, waits for a random time
  then gives a prompt for the user to click enter to measure
  their reaction time.

  If the reaction time is "highscore", then write it to file
 */
void reaction_game();

//Display the current highscore from "highscore.txt" (if it exists)
void see_highscore();

//Writes highscore variable (obtained from reaction_game function) to "highscore.txt"
void write_highscore_to_file(double highscore);

/*
  Returns the current highscore from "highscore.txt"

  Returns -1 if their is an error opening the file (meaning they have never played before)
  Returns -2 if the file exists but there was an error reading the highscore
      - If called_by_play, then there is no error, and the highscore was simply cleared
*/
double read_highscore_from_file(bool called_by_play);

//Clear "highscore.txt" if it exists
void clear_highscore();

int main(void) {
	srand(time(NULL));

	how_to_play();

	char user_input[3];
	bool user_quit = false;
	
	while (!user_quit) {
		printf("> ");
		scanf("%2s", user_input);
		
		if (strcmp(user_input, "-s") == 0) {
			see_highscore();
		} else if (strcmp(user_input, "-p") == 0) {
			reaction_game();
		} else if (strcmp(user_input, "-h") == 0) {
			print_commands();
		} else if (strcmp(user_input, "-c") == 0) {
			clear_highscore();
		} else if (strcmp(user_input, "-q") == 0) {
			user_quit = true;
		} else {
			printf("Error: invalid command.\n");
		}
	}
}

void print_commands() {
	printf("=Commands=\n");
	printf("----------------------------------------------------\n");
	printf("-s: view highscore\n-p: play\n-h: instructions\n-c: clear highscore\n-q: to quit\n");
	printf("----------------------------------------------------\n");
}

void how_to_play() {
	printf("\n=Instructions=\n");
	printf("--------------\n");
	printf("After the countdown, the console will clear\n");
	printf("After a random duration, a prompt will appear\n");
	printf("Press [ENTER] once you see the prompt\n");
	printf("-h to see all commands.\n");
	printf("----------------\n");
}

void reaction_game() {
	struct timeval timer_start, timer_end;

	system("clear");
	for (int i = 3; i > 0; i--) {
		printf("%d...\n", i);
		sleep(1);
	}
	system("clear");
	printf("Wait...\n");
	
	int time_min = 1, time_max = 6;
	int time_before_prompt = rand() % (time_max - time_min + 1) + time_min;

	sleep(time_before_prompt);
	gettimeofday(&timer_start, NULL);

	char c;
	while ((c = getchar()) != '\n' && c != EOF);
	
	printf("Press Enter!\n");
	
	while ((c = getchar()) != '\n');

	gettimeofday(&timer_end, NULL);

	double time_taken = (timer_end.tv_sec - timer_start.tv_sec)
		+ (timer_end.tv_usec - timer_start.tv_usec) / 1000000.0;
	
	printf("Time: %lf\n", time_taken);
	
	double old_highscore = read_highscore_from_file(true);

	//Would be less than 0 if the file doesn't exist, or was cleared with -c
	if (old_highscore < 0 || old_highscore > time_taken) {
		printf("New Highscore! Writing to highscore.txt\n");
		write_highscore_to_file(time_taken);
	}
}

void write_highscore_to_file(double highscore) {
	
	FILE * highscore_file = fopen(FILE_NAME, "w");
	if (!highscore_file) {
		printf("Error: could not open highscore.txt\n");
		fclose(highscore_file);
		return;
	}

	fprintf(highscore_file, "%f", highscore);
	fclose(highscore_file);

	return;
}

double read_highscore_from_file(bool called_by_play) {
	FILE * highscore_file = fopen(FILE_NAME, "r");

	if (!highscore_file) {
		return -1;
	}

	double value;
	if (fscanf(highscore_file, "%lf", &value) == 1) {
		fclose(highscore_file);
		return value;
	} else {
		//If called by game loop, dont display error, as value may have been cleared with -c
		if (!called_by_play) {
			printf("Error: highscore.txt is empty. Must play to get a highscore.\n");
		}
		fclose(highscore_file);
		return -2;
	}
}

void see_highscore() {
	double highscore = read_highscore_from_file(false);

	if ((int) highscore == -1) {
		printf("Error: highscore.txt does not exist. Must play first.\n");
		return;
	} else if ((int) highscore == -2) {
		return;
	}

	printf("Highscore: %lf\n", highscore);
}

void clear_highscore() {

	int return_val = (int) read_highscore_from_file(false);
	if (return_val == -1) {
		printf("Error: highscore.txt does not exist. Must play first.\n");
		return;
	} else if (return_val == -2) {
		return;
	}

	FILE * highscore = fopen(FILE_NAME, "w");
	fclose(highscore);

	printf("Highscore reset.\n");
}

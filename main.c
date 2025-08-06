#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//TODO 
//FINISH THE OVERRIDE FUNTIONALITY

//TERMINAL STUFF
#include <sys/ioctl.h>
#include <unistd.h>

int alreadyDidToday(const char* path, char* now){
    //opening file
    FILE* fp = fopen(path, "r");
    char line[1024];
    char lastLine[1024] = "";

    //looping until the last line, saving every previous last line
    while(fgets(line, sizeof(line), fp)){
        strcpy(lastLine, line);
    }

    fclose(fp);

    //grab the last date saved in the file
    char LastDate[6];
    strncpy(LastDate, lastLine, 5);
    LastDate[5] = '\0'; //i am da terminator 

    //grab the current date 
    char nowDate[6];
    strncpy(nowDate, now,  5);
    nowDate[5] = '\0'; //i am da terminator

    //compare them and return 0 if they are the same, meaning we already entered a value for today.
    printf("%s, %s\n", nowDate, LastDate);
    if(strcmp(nowDate, LastDate) == 0){
        return 1;
    }
    

    //return -1 meaning we have no value for today
    printf("returning -1\n");
    return 0;
}



int OverRide(const char* filepath){
    printf("You already entered a value for today\n would you like to override it? (y/n)");
    char c = getchar();

    if(c == 'y'){
        FILE* fp = fopen(filepath, "w");

        char line[1024];
        char lastLine[1024] = "";

        //looping until the last line, saving every previous last line
        while(fgets(line, sizeof(line), fp)){
            strcpy(lastLine, line);
        }        

        return 0;
    }
    else{
        return -1;
    }
}

int dayRating(int value){
    //if the user inputs a value for the day
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char date[64];
    char fullpath[128];

    //grab the home env
    const char* home = getenv("HOME");

    strftime(date, sizeof(date), "%d-%m-%y_%H:%M_%B_%A", timeinfo);

    if(-1 > value || 9 < value){
        printf("NOTVALID \n");
        return -1;
    }

    printf("inputting data\n");

    //some funky buisiness
    snprintf(fullpath, sizeof(fullpath), "%s/Documents/DayByDay",home);


    if(alreadyDidToday(fullpath, date)){
        printf("You have already entered a value for today, it must be overriten maually with nano\n");
        printf("functionaliity to come\n");
        return 1;
    }

    FILE *fp = fopen(fullpath,  "a");

    fprintf(fp, "%s: %d", date, value);


    return 0;
}

int get_terminal_size() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return -1;
    }
    return w.ws_col;
}

int display(){
    printf("Displaying\nKey:\n");
    printf(
        "\033[41m  \033[0m"     // red bg
        "\033[48;5;208m  \033[0m"  // orange bg (256 color)
        "\033[43m  \033[0m"     // yellow bg
        "\033[100m  \033[0m"    // bright gray bg
        "\033[40m  \033[0m"     // black bg
        "\033[47m  \033[0m"     // white bg
        "\033[44m  \033[0m"     // blue bg
        "\033[46m  \033[0m"     // cyan bg
        "\033[45m  \033[0m"     // magenta bg
        "\033[42m  \033[0m"     // green bg
        "\n"
        );
    printf("1 2 3 4 5 6 7 8 9 0\n\n\n");

    //grab the home env
    const char* home = getenv("HOME");
    char fullpath[128];

    snprintf(fullpath, sizeof(fullpath), "%s/Documents/DayByDay",home);
    FILE *fp = fopen(fullpath ,"r");

    char line[1024];
    int count = 0;

    int terminalSize = get_terminal_size();
    

    while(fgets(line, sizeof(line), fp)){
        char* rating = strchr(line, ' ') + 1;
        //printf("rating: %c\n", *rating);
        int ratingINT = atoi(rating);

        if(count == terminalSize/2){
            printf("\n");
        }
        
        switch (ratingINT) {
            case 1:
                printf("\033[41m  \033[0m");  // Red background
                break;
            case 2:
                printf("\033[48;5;208m  \033[0m");  // Orange background
                break;
            case 3:
                printf("\033[43m  \033[0m");  // Yellow background
                break;
            case 4:
                printf("\033[100m  \033[0m");  // Bright gray background
                break;
            case 5:
                printf("\033[40m  \033[0m");  // Black background
                break;
            case 6:
                printf("\033[47m  \033[0m");  // White background
                break;
            case 7:
                printf("\033[44m  \033[0m");  // Blue background
                break;
            case 8:
                printf("\033[46m  \033[0m");  // Cyan background
                break;
            case 9:
                printf("\033[45m  \033[0m");  // Magenta background
                break;
            case 10:  // You said 10 maps to 0
            case 0:
                printf("\033[42m  \033[0m");  // Green background
                break;
            default:
                printf("  ");  // Empty block for unknown ratings
                break;
        }
        count++;
    }
    printf("\n");
    return 0;;
}

int main(int argc, char* argv[]){

    if(argc != 2){
        printf("Include rating or -d flag to display\n");
        return -1;
    }

    int value;
    char* arg = argv[1];

    if(strcmp(arg, "-d") == 0){
        value = display();
    }
    else{
        value = atoi(argv[1]);
        dayRating(value);
    }


    return 0;

}
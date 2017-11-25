/*
 * A little terminal-based program for getting the weekday of the actual day
 * or for an user defined date.
 *
 * It's written in C and is originated from an exercise at the University of Innsbruck.
 *
 * Author: Philip "Zeruxky" Wille
 * Created at: 25.11.2017
 * Version: 1.0.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

//Constants for calculation, queries and so on
int const START_DAY = 1;
int const START_MONTH = 1;
int const START_YEAR = 1970;
int const MAX_MONTHS = 12;

//Looking better in code and is more realistic --> January is first month; February second and so on
enum months {
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

//Looks also better in code; beginning with thursday, because the 1970-1-1 is a thursday
enum days {
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY
};

//For getting the actual time
struct tm *time_val;
time_t unix_time;

int calculateDifferenceYears(int);
int calculateDifferenceMonths(int, int*, int);
int calculateDifferenceDays(int, int);

int main(void) {
    int inputDay = START_DAY; //if the user make no inputs, the day will calculated for the 1970-1-1
    int inputMonth = START_MONTH;
    int inputYear = START_YEAR;
    int amountOfYears = 0;
    int amountOfMonths = 0;
    int amountOfDays = 0;
    int yearForCalculation = 0;
    int maxDaysAtMonth[MAX_MONTHS]; //for getting the maximum days at the months
    char tempInput, userInput;
    bool isDayInputCorrect = false;
    bool isProgramRunning = true;
    bool isYearLeap = false;
    struct tm *time_val;
    time_t unix_time;
    unix_time = time(NULL);
    time_val = localtime(&unix_time);

    do {
        printf("Do you want to enter a date [Y] or get the actual weekday [N]?\n");
        scanf("%c%c", &userInput, &tempInput);
        if(toupper(userInput) == 'Y') {
            printf("Enter a date before 1970-1-1 (format: YYYY-MM-TT)\n");
            printf("Year: ");
            scanf("%d%c", &inputYear, &tempInput);
            printf("Month: ");
            scanf("%d%c", &inputMonth, &tempInput);
            printf("Day: ");
            scanf("%d%c", &inputDay, &tempInput);
        }
        else {
            if (time_val != NULL) {
                inputMonth = time_val->tm_mon + 1;
                inputDay = time_val->tm_mday;
                inputYear = time_val->tm_year + 1900;
            }
        }
        if((inputDay >= 1)
           &&(inputDay <= 31)
           &&(inputMonth != FEBRUARY)
           &&(inputMonth >= JANUARY)
           &&(inputMonth <= DECEMBER)
           &&(inputYear >= 0)) { //proving if input is correct
            isDayInputCorrect = true;
        }
        else if((inputDay >= 1&&inputDay <= 29)&&(inputMonth == FEBRUARY)) { //also proving if input is correct --> Leap year will checked later
            isDayInputCorrect = true;
        }
        else {
            isDayInputCorrect = false;
            printf("Wrong input! Please enter again.\n");
        }
        if(isDayInputCorrect == true) {
            yearForCalculation = inputYear;
            if((yearForCalculation%4 == 0)
               ||(yearForCalculation%400 == 0)) { //check if the year is a leap year
                if(yearForCalculation%100 == 0) { //avoid the non normal case --> is no leap year
                    isYearLeap = false;
                }
                else {
                    isYearLeap = true;
                }
            }
            for(int i = 1; i <= MAX_MONTHS; i++) { //init the array for getting the maximum days at the month
                if((i == JULY)||(i == AUGUST)) {
                    maxDaysAtMonth[i-1] = 31;
                }
                else if((i%2 == 0)&&(i < JULY)) {
                    if(i == FEBRUARY) {
                        if(isYearLeap == true) {
                            maxDaysAtMonth[i-1] = 29;
                        }
                        else {
                            maxDaysAtMonth[i-1] = 28;
                        }
                    }
                    else {
                        maxDaysAtMonth[i-1] = 30;
                    }
                }
                else if((i%2 == 1)&&(i >= JULY)) {
                    maxDaysAtMonth[i - 1] = 30;
                }
                else {
                    maxDaysAtMonth[i-1] = 31;
                }
            }
            amountOfYears = calculateDifferenceYears(yearForCalculation);
            amountOfMonths = calculateDifferenceMonths(inputMonth, maxDaysAtMonth, amountOfYears);
            amountOfDays = calculateDifferenceDays(inputDay, amountOfMonths);
            if(amountOfDays < 0) { //if the entered year is before the start year
                amountOfDays += 7;
            }
            if(amountOfDays >= 0) {
                printf("The day for %d-%d-%d is: ",  inputYear, inputMonth, inputDay);
                switch(amountOfDays) {
                    case THURSDAY:
                        printf("Thursday");
                        break;
                    case FRIDAY:
                        printf("Friday");
                        break;
                    case SATURDAY:
                        printf("Saturday");
                        break;
                    case SUNDAY:
                        printf("Sunday");
                        break;
                    case MONDAY:
                        printf("Monday");
                        break;
                    case TUESDAY:
                        printf("Tuesday");
                        break;
                    case WEDNESDAY:
                        printf("Wednesday");
                        break;
                }
                printf("\n");
            }
            else {
                printf("Wrong input! The entered year is no leap year.\n");
            }
            printf("Do you want to enter a new date? [Y/N]\n");
            userInput = getchar();
            if(toupper(userInput) == 'N') {
                isProgramRunning = false;
            }
        }
    }while(isProgramRunning == true);
    return EXIT_SUCCESS;
}

/*
 * Calculate the difference between entered year and start year
 * Gives the number of shifts for the days at the xxxx-1-1
 * @int year
 * @return int amountOfYears
 */
int calculateDifferenceYears(int year) {
    int shiftingCounter = 0;
    int amountOfYear = 0;
    if(year >= START_YEAR) { // all dates after 1970-1-1 beginning with date 1970-1-1
        for(int i = START_YEAR; i < year; i++) {
            if((i%4 == 0)||(i%400 == 0)) {
                if(i%100 == 1) {
                    shiftingCounter++; //if no leap year -> count one up
                }
                else{
                    shiftingCounter += 2; //if leap year -> count two up
                }
            }
            else {
                shiftingCounter++; //if no leap year -> count one up
            }
        }
    }
    else { //all dates past 1970-1-1
        int tempYear = START_YEAR - year;
        for(int i = 0; i < tempYear; i++) {
            if((year%4 == 0)||(year%400 == 0)) {
                if(year%100 == 1) {
                    shiftingCounter--; //if no leap year -> count one up
                }
                else{
                    shiftingCounter -= 2; //if leap year -> count two up
                }
            }
            else {
                shiftingCounter--; //if no leap year -> count one up
            }
        }
    }
    amountOfYear = (shiftingCounter%7); //calculate the needed shifts
    return amountOfYear;
}

/*
 * Calculate the difference between entered month and start month
 * Gives the number of shifts for the days at the xxxx-x-1
 * @int month, int* maxDays, int amountOfYears
 * @return int amountOfMonths
 */
int calculateDifferenceMonths(int month, int* maxDays, int amountOfYears) {
    int differenceMonths = month - START_MONTH; //calculate difference
    int shiftingCounter = 0;
    int amountOfMonths = 0;
    for(int i = 0; i < differenceMonths; i++) {
        shiftingCounter += maxDays[i]; //get the days between the entered month and the start month
    }
    amountOfMonths = (shiftingCounter%7) + amountOfYears; //calculate the needed shifts
    return amountOfMonths;
}

/*
 * Calculate the difference between entered day and start day
 * Gives the number of shifts for the days at the xxxx-x-x
 * @int day, int amountOfMonths
 * @return int amountOfDays
 */
int calculateDifferenceDays(int day, int amountOfMonths) {
    int differenceDays = day - START_DAY; //calculate difference
    int amountOfDays = ((differenceDays) + amountOfMonths)%7;
    return amountOfDays;
}
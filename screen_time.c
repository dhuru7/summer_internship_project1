#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define FILE_NAME "screen_time.txt"

// Convert seconds into hours, minutes and seconds
void displayTime(long seconds)
{
    int hours;
    int minutes;
    int secondsLeft;

    hours = seconds / 3600;
    minutes = (seconds % 3600) / 60;
    secondsLeft = seconds % 60;

    printf("%02d:%02d:%02d", hours, minutes, secondsLeft);
}

// Get today's date
void getDate(int *day, int *month, int *year)
{
    time_t currentTime;
    struct tm *currentDate;

    currentTime = time(NULL);
    currentDate = localtime(&currentTime);

    *day = currentDate->tm_mday;
    *month = currentDate->tm_mon + 1;
    *year = currentDate->tm_year + 1900;
}

// Find today's saved time
long getTodayTime(int day, int month, int year)
{
    FILE *file;
    int savedDay;
    int savedMonth;
    int savedYear;
    long savedSeconds;

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fscanf(file, "%d %d %d %ld",
                  &savedDay,
                  &savedMonth,
                  &savedYear,
                  &savedSeconds) == 4)
    {
        if (savedDay == day &&
            savedMonth == month &&
            savedYear == year)
        {
            fclose(file);
            return savedSeconds;
        }
    }

    fclose(file);

    return 0;
}

// Save today's time
void saveToday(int day, int month, int year, long totalSeconds)
{
    FILE *file;
    FILE *temp;
    int savedDay;
    int savedMonth;
    int savedYear;
    long savedSeconds;
    int found = 0;

    file = fopen(FILE_NAME, "r");
    temp = fopen("temp.txt", "w");

    if (temp == NULL)
    {
        printf("\nCould not save data.\n");
        return;
    }

    // Copy old records into temporary file
    if (file != NULL)
    {
        while (fscanf(file, "%d %d %d %ld",
                      &savedDay,
                      &savedMonth,
                      &savedYear,
                      &savedSeconds) == 4)
        {
            if (savedDay == day &&
                savedMonth == month &&
                savedYear == year)
            {
                // Replace today's old value
                fprintf(temp, "%d %d %d %ld\n",
                        day, month, year, totalSeconds);

                found = 1;
            }
            else
            {
                // Keep old record
                fprintf(temp, "%d %d %d %ld\n",
                        savedDay,
                        savedMonth,
                        savedYear,
                        savedSeconds);
            }
        }

        fclose(file);
    }

    // If today's date wasn't already in the file
    if (found == 0)
    {
        fprintf(temp, "%d %d %d %ld\n",
                day, month, year, totalSeconds);
    }

    fclose(temp);

    // Replace old file with new file
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
}

// Display saved history
void showHistory()
{
    FILE *file;

    int day;
    int month;
    int year;
    long seconds;

    printf("\n============================================\n");
    printf("             SCREEN TIME HISTORY\n");
    printf("============================================\n\n");

    file = fopen(FILE_NAME, "r");

    if (file == NULL)
    {
        printf("No screen time history available.\n");
        printf("============================================\n");
        return;
    }

    printf("Date            Screen Time\n");
    printf("--------------------------------------------\n");

    while (fscanf(file, "%d %d %d %ld",
                  &day,
                  &month,
                  &year,
                  &seconds) == 4)
    {
        printf("%02d-%02d-%04d      ",
               day, month, year);

        displayTime(seconds);

        printf("\n");
    }

    fclose(file);

    printf("============================================\n");
}

int main()
{
    time_t startTime;
    time_t currentTime;

    long previousTime;
    long todayTime;

    int day;
    int month;
    int year;

    int currentDay;
    int currentMonth;
    int currentYear;

    char command;

    // Get today's date
    getDate(&day, &month, &year);

    // Load previously saved time for today
    todayTime = getTodayTime(day, month, year);

    // Record the time when program starts
    startTime = time(NULL);

    previousTime = (long)startTime;

    printf("============================================\n");
    printf("          DESKTOP SCREEN TIME TRACKER\n");
    printf("============================================\n\n");

    printf("Date: %02d-%02d-%04d\n\n",
           day, month, year);

    printf("Screen Time: ");
    displayTime(todayTime);

    printf("\n\n");
    printf("H = View History\n");
    printf("Q = Quit\n\n");

    printf("Timer started...\n");

    while (1)
    {
        Sleep(1000);

        // Get current time
        currentTime = time(NULL);

        // Add elapsed seconds
        if ((long)currentTime > previousTime)
        {
            todayTime = todayTime +
                        ((long)currentTime - previousTime);

            previousTime = (long)currentTime;
        }

        // Check whether the date has changed
        getDate(&currentDay, &currentMonth, &currentYear);

        if (currentDay != day ||
            currentMonth != month ||
            currentYear != year)
        {
            // Save yesterday's record
            saveToday(day, month, year, todayTime);

            // Start new day
            day = currentDay;
            month = currentMonth;
            year = currentYear;

            todayTime = getTodayTime(day, month, year);
        }

        // Display current timer
        printf("\rScreen Time: ");
        displayTime(todayTime);
        fflush(stdout);

        // Check keyboard input
        if (_kbhit())
        {
            command = _getch();

            if (command == 'h' || command == 'H')
            {
                showHistory();

                printf("\nPress any key to return to timer...");
                _getch();

                printf("\n\nScreen Time: ");
                displayTime(todayTime);
                fflush(stdout);
            }

            else if (command == 'q' || command == 'Q')
            {
                // Save today's screen time
                saveToday(day, month, year, todayTime);

                printf("\n\n============================================\n");
                printf("Today's screen time has been saved.\n");
                printf("Final Screen Time: ");

                displayTime(todayTime);

                printf("\n============================================\n");

                break;
            }
        }
    }

    return 0;
}
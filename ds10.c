#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 5
#define MAX_ROWS 10
#define MAX_COLS 10

typedef struct {
    int row;
    int col;
    int isBooked;
} Seat;

typedef struct {
    int movieID;
    char title[50];
    char date[15];
    char time[10];
    float price;
    Seat seats[MAX_ROWS][MAX_COLS];
} Movie;

typedef struct {
    char username[30];
    char password[30];
} User;

typedef struct {
    char username[30];
    int movieID;
    int row;
    int col;
} Booking;

// Function prototypes

void addMovies(Movie movies[], int *movieCount);
void viewMovies(Movie movies[], int movieCount);
void viewAvailableSeats(Movie movies[], int movieCount);
void bookTicket(Movie movies[], int movieCount, char *username);
void saveBooking(Booking booking);
void viewUserTransactions(char *username);
void saveUser(User user);
int loginUser(char *username, char *password);

// Admin functions

int adminLogin();
void adminPanel(Movie movies[], int *movieCount);
void addNewMovie(Movie movies[], int *movieCount);
void deleteMovie(Movie movies[], int *movieCount);
void updateMovieDetails(Movie movies[], int movieCount);
void resetSeatAvailability(Movie movies[], int movieCount);
void viewAllBookings();

// User functions

void registerUser();
void userPanel(Movie movies[], int movieCount, char *username);

int main() {
    Movie movies[MAX_MOVIES];
    int movieCount = 0;
    int choice;

    // Initialize movies and add sample movies

    addMovies(movies, &movieCount);

    do {
        printf("\n                   ------------------------------------------------");
        printf("\n                             Movie Ticket Booking System          \n");
        printf("                   -----------------------------------------------\n\n");
        
        printf("1. Admin Login\n");
        printf("2. User Login\n");
        printf("3. Register\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (adminLogin()) {
                    adminPanel(movies, &movieCount);
                } else {
                    printf("Invalid Admin Login.\n");
                }
                break;

            case 2: {
                char username[30], password[30];
                printf("Enter Username: ");
                scanf("%s", username);
                printf("Enter Password: ");
                scanf("%s", password);

                if (loginUser(username, password)) {
                    userPanel(movies, movieCount, username);
                } else {
                    printf("Invalid Username or Password.\n");
                }
                break;
            }
            case 3:
                registerUser();
                break;

            case 4:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}

// Register a new user

void registerUser() {
    system("clear");
    User user;
    printf("\nEnter a Username: ");
    scanf("%s", user.username);
    printf("Enter a Password: ");
    scanf("%s", user.password);

    saveUser(user);
    printf("Registration successful! You can now login.\n");
}

// Save user details to a file

void saveUser(User user) {
    system("clear");
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error saving user data.\n");
        return;
    }

    fprintf(file, "%s %s\n", user.username, user.password);
    fclose(file);
}

// Validate user login

int loginUser(char *username, char *password) {
    system("clear");
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("No registered users found.\n");
        return 0;
    }

    char storedUsername[30], storedPassword[30];
    while (fscanf(file, "%s %s", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// User panel functionality

void userPanel(Movie movies[], int movieCount, char *username) {
    system("clear");
    int userChoice;
    do {
        printf("\nUser Panel:\n");
        printf("1. View Movies\n");
        printf("2. View Available Seats\n");
        printf("3. Book Ticket\n");
        printf("4. View My Transactions\n");
        printf("5. Exit User Panel\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        switch (userChoice) {
            case 1:
                viewMovies(movies, movieCount);
                break;
            case 2:
                viewAvailableSeats(movies, movieCount);
                break;
            case 3:
                bookTicket(movies, movieCount, username);
                break;
            case 4:
                viewUserTransactions(username);
                break;
            case 5:
                printf("Exiting User Panel...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (userChoice != 5);
}

// Enhanced seat viewing

void viewAvailableSeats(Movie movies[], int movieCount) {
    system("clear");
    int movieID;
    printf("\nEnter the Movie ID to view available seats: ");
    scanf("%d", &movieID);

    if (movieID <= 0 || movieID > movieCount) {
        printf("Invalid Movie ID.\n");
        return;
    }

    printf("\nAvailable Seats for '%s':\n", movies[movieID - 1].title);

    printf("   ");
    for (int col = 0; col < MAX_COLS; col++) {
        printf("%3d", col);
    }
    printf("\n");

    for (int row = 0; row < MAX_ROWS; row++) {
        printf("%3d", row);
        for (int col = 0; col < MAX_COLS; col++) {
            printf("%3s", movies[movieID - 1].seats[row][col].isBooked ? "[X]" : "[ ]");
        }
        printf("\n");
    }
}

// Book a ticket

void bookTicket(Movie movies[], int movieCount, char *username) {
    system("clear");
    int movieID, row, col;

    printf("\nEnter Movie ID to book a ticket: ");
    scanf("%d", &movieID);

    if (movieID <= 0 || movieID > movieCount) {
        printf("Invalid Movie ID.\n");
        return;
    }

    printf("Enter Row (0-%d): ", MAX_ROWS - 1);
    scanf("%d", &row);
    printf("Enter Column (0-%d): ", MAX_COLS - 1);
    scanf("%d", &col);

    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
        printf("Invalid seat selection.\n");
        return;
    }

    if (movies[movieID - 1].seats[row][col].isBooked) {
        printf("Seat is already booked. Try a different one.\n");
    } else {
        movies[movieID - 1].seats[row][col].isBooked = 1;
        printf("Booking successful for %s (Row: %d, Col: %d)\n", movies[movieID - 1].title, row, col);

        Booking booking = {0};
        strcpy(booking.username, username);
        booking.movieID = movieID;
        booking.row = row;
        booking.col = col;

        saveBooking(booking);
    }
}

// Save booking details to file

void saveBooking(Booking booking) {
    system("clear");
    FILE *file = fopen("bookings.txt", "a");
    if (file == NULL) {
        printf("Error saving booking.\n");
        return;
    }

    fprintf(file, "%s %d %d %d\n", booking.username, booking.movieID, booking.row, booking.col);
    fclose(file);
    printf("Booking saved successfully.\n");
}

// View user transactions

void viewUserTransactions(char *username) {
    system("clear");
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("No transactions found.\n");
        return;
    }

    char fileUsername[30];
    int movieID, row, col;
    int found = 0;

    printf("\nTransactions for user : '%s'\n", username);
    while (fscanf(file, "%s %d %d %d", fileUsername, &movieID, &row, &col) != EOF) {
        if (strcmp(fileUsername, username) == 0) {
            printf("Movie ID: %d \nBooked Seat: [Row %d, Col %d]\n", movieID, row, col);
            printf("-----------------------------------");
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found.\n");
    }

    fclose(file);
}

// View all movies

void viewMovies(Movie movies[], int movieCount) {
    system("clear");
    printf("\nAvailable Movies:\n");
    for (int i = 0; i < movieCount; i++) {
        printf("Movie ID: %d\n", movies[i].movieID);
        printf("Title   : %s\n", movies[i].title);
        printf("Date    : %s\n", movies[i].date);
        printf("Time    : %s\n", movies[i].time);
        printf("Price   : %.2fTk\n", movies[i].price);
        printf("-----------------------------\n");
    }
}

// Add initial sample movies

void addMovies(Movie movies[], int *movieCount) {
    system("clear");
    strcpy(movies[0].title, "Inception");
    strcpy(movies[0].date, "2024-12-04");
    strcpy(movies[0].time, "7:30 PM");
    movies[0].price = 850;
    movies[0].movieID = 1;

    strcpy(movies[1].title, "Avengers");
    strcpy(movies[1].date, "2024-11-05");
    strcpy(movies[1].time, "9:00 PM");
    movies[1].price = 1000;
    movies[1].movieID = 2;

    strcpy(movies[2].title, "Iron Man 3");
    strcpy(movies[2].date, "2024-11-06");
    strcpy(movies[2].time, "07:00 PM");
    movies[2].price = 950;
    movies[2].movieID = 3;

    *movieCount = 3;

    // Initialize seats for each movie

    for (int m = 0; m < *movieCount; m++) {
        for (int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; j < MAX_COLS; j++) {
                movies[m].seats[i][j].row = i;
                movies[m].seats[i][j].col = j;
                movies[m].seats[i][j].isBooked = 0;
            }
        }
    }
}

// Admin login verification

int adminLogin() {
    system("clear");
    char adminUsername[30], adminPassword[30];

    printf("\nAdmin Username: ");
    scanf("%s", adminUsername);
    printf("Admin Password: ");
    scanf("%s", adminPassword);

    return (strcmp(adminUsername, "admin") == 0 && strcmp(adminPassword, "123") == 0);
}

// Admin panel

void adminPanel(Movie movies[], int *movieCount) {
    system("clear");
    int adminChoice;

    do {
        printf("\nAdmin Panel:\n");
        printf("1. Add New Movie\n");
        printf("2. Delete Movie\n");
        printf("3. Update Movie Details\n");
        printf("4. Reset Seat Availability\n");
        printf("5. View All Bookings\n");
        printf("6. Exit Admin Panel\n");
        printf("Enter your choice: ");
        scanf("%d", &adminChoice);

        switch (adminChoice) {
            case 1:
                addNewMovie(movies, movieCount);
                break;
            case 2:
                deleteMovie(movies, movieCount);
                break;
            case 3:
                updateMovieDetails(movies, *movieCount);
                break;
            case 4:
                resetSeatAvailability(movies, *movieCount);
                break;
            case 5:
                viewAllBookings();
                break;
            case 6:
                printf("Exiting Admin Panel...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (adminChoice != 6);
}

// Add new movie

void addNewMovie(Movie movies[], int *movieCount) {
    system("clear");
    if (*movieCount >= MAX_MOVIES) {
        printf("Cannot add more movies. Maximum limit reached.\n");
        return;
    }

    printf("\nEnter Movie Title: ");
    scanf(" %[^\n]", movies[*movieCount].title);
    printf("Enter Movie Date (YYYY-MM-DD): ");
    scanf("%s", movies[*movieCount].date);
    printf("Enter Movie Time (HH:MM AM/PM): ");
    scanf("%s", movies[*movieCount].time);
    printf("Enter Ticket Price: ");
    scanf("%f", &movies[*movieCount].price);

    movies[*movieCount].movieID = *movieCount + 1;

    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            movies[*movieCount].seats[i][j].row = i;
            movies[*movieCount].seats[i][j].col = j;
            movies[*movieCount].seats[i][j].isBooked = 0;
        }
    }

    (*movieCount)++;
    printf("New movie added successfully!\n");
}

// Delete movie

void deleteMovie(Movie movies[], int *movieCount) {
    system("clear");
    int movieID;
    printf("\nEnter Movie ID to delete: ");
    scanf("%d", &movieID);

    if (movieID <= 0 || movieID > *movieCount) {
        printf("Invalid Movie ID.\n");
        return;
    }

    for (int i = movieID - 1; i < *movieCount - 1; i++) {
        movies[i] = movies[i + 1];
    }

    (*movieCount)--;
    printf("Movie deleted successfully!\n");
}

// Update movie details

void updateMovieDetails(Movie movies[], int movieCount) {
    system("clear");
    int movieID;
    printf("\nEnter Movie ID to update details: ");
    scanf("%d", &movieID);

    if (movieID <= 0 || movieID > movieCount) {
        printf("Invalid Movie ID.\n");
        return;
    }

    printf("Updating details for Movie ID %d (%s):\n", movieID, movies[movieID - 1].title);

    printf("Enter new Title (or press Enter to keep '%s'): ", movies[movieID - 1].title);
    getchar(); // Consume newline
    char newTitle[50];
    if (fgets(newTitle, sizeof(newTitle), stdin) != NULL && strcmp(newTitle, "\n") != 0) {
        newTitle[strcspn(newTitle, "\n")] = '\0'; // Remove trailing newline
        strcpy(movies[movieID - 1].title, newTitle);
    }

    printf("Enter new Date (or press Enter to keep '%s'): ", movies[movieID - 1].date);
    char newDate[15];
    if (fgets(newDate, sizeof(newDate), stdin) != NULL && strcmp(newDate, "\n") != 0) {
        newDate[strcspn(newDate, "\n")] = '\0'; // Remove trailing newline
        strcpy(movies[movieID - 1].date, newDate);
    }

    printf("Enter new Time (or press Enter to keep '%s'): ", movies[movieID - 1].time);
    char newTime[10];
    if (fgets(newTime, sizeof(newTime), stdin) != NULL && strcmp(newTime, "\n") != 0) {
        newTime[strcspn(newTime, "\n")] = '\0'; // Remove trailing newline
        strcpy(movies[movieID - 1].time, newTime);
    }

    printf("Enter new Ticket Price (or press Enter to keep %.2f): ", movies[movieID - 1].price);
    char newPrice[10];
    if (fgets(newPrice, sizeof(newPrice), stdin) != NULL && strcmp(newPrice, "\n") != 0) {
        newPrice[strcspn(newPrice, "\n")] = '\0'; // Remove trailing newline
        movies[movieID - 1].price = atof(newPrice);
    }

    printf("Movie details updated successfully!\n");
}

// Reset seat availability

void resetSeatAvailability(Movie movies[], int movieCount) {
    system("clear");
    int movieID;
    printf("\nEnter Movie ID to reset seat availability: ");
    scanf("%d", &movieID);

    if (movieID <= 0 || movieID > movieCount) {
        printf("Invalid Movie ID.\n");
        return;
    }

    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            movies[movieID - 1].seats[i][j].isBooked = 0;
        }
    }

    printf("Seat availability reset successfully!\n");
}

// View all bookings

void viewAllBookings() {
    system("clear");
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("No bookings found.\n");
        return;
    }

    char username[30];
    int movieID, row, col;

    printf("\nAll Bookings:\n");
    while (fscanf(file, "%s %d %d %d", username, &movieID, &row, &col) != EOF) {
        printf("User: %s, Movie ID: %d, Seat: Row %d, Col %d\n", username, movieID, row, col);
    }

    fclose(file);
}

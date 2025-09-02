#include <stdio.h>
#include <stdlib.h>
#include <string.h> //  functions like strcpy, strcmp, strlen
#include <windows.h>  // Provides access to Windows system-level functions.
#include <mmsystem.h> // Required for multimedia functions like PlaySound() to play audio files.

#pragma comment(lib, "winmm.lib") // pragma - pre processor directive
// This tells the compiler to link with the winmm.lib library, which is necessary to use PlaySound() and other Windows multimedia features.

typedef struct Song {
    char name[100]; // Stores the name of the song
    struct Song* next; // Pointer to the next song 
    struct Song* prev; // Pointer to the previous song
} Song;


Song *head = NULL, *tail = NULL, *current = NULL;
// head -- points to the first song 
// tail -- points to last song
// current -- points to currently playing or selested song

// This function writes the current playlist (linked list of songs) to a file — typically playlist.txt.
// It ensures that any songs you add or remove in the program are saved permanently to disk.
void savePlaylistToFile(const char *filename) {
    //FILE is a structure (struct) used by the standard library to represent a file stream (open file).

    FILE *file = fopen(filename, "w");
    // file -- pointer to file stream
    // fopen() -- function to open file

    if (!file) {
        printf("Error saving playlist.\n");
        return;
    }
    Song *temp = head;
    //Initializes a pointer temp to start at the head of the linked list.
    //It will be used to traverse the playlist.  

    // the loop will write each sing name to the file , one song per line 
    while (temp != NULL) {
        fprintf(file, "%s\n", temp->name); // fprintf works like printf but writes to a file
        temp = temp->next; // move to next song 
    }
    fclose(file); // function to close the file
}

// function to add song
void addSong(char *name) {
    Song *newSong = (Song *)malloc(sizeof(Song)); // Allocate memory
    strcpy(newSong->name, name); // copies the file name passed as argument in the name variable (from structure)
    newSong->next = NULL; // currently last song
    newSong->prev = tail; // links the new song to last song

    if (head == NULL) { // is list is empty
        head = tail = current = newSong; 
    } else {
        tail->next = newSong; // Update tail to point to the new song.
        tail = newSong;
    }

    savePlaylistToFile("playlist.txt");
    printf("Added: %s\n\n", name);
}


// checks if file with given name exists 

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r"); // r - means read mode
    if (file) { // if not null means file exist
        fclose(file); // Since we just needed to check for existence, we close the file immediately.
        return 1;  // indicates that file exists
    }
    return 0;
}


void playCurrent() {
    // no song to play
    if (current == NULL) {
        printf("No song to play.\n\n");
        return;
    }
    // file does not exist
    if (!fileExists(current->name)) {
        printf("File not found: %s\n\n", current->name);
        return;
    }
    // else playing song 
    printf("Now playing: %s\n\n", current->name);

    PlaySound(current->name, NULL, SND_FILENAME | SND_ASYNC);
    // First argument -- filename
    // second -- no custom sound module
    // third -- 
            // SND_FILENAME --  Tells the PlaySound() function that the first argument (current->name) is a path to a .wav file, not a built-in Windows sound or resource.
            // SND_ASYNC --  Tells PlaySound() to play the sound asynchronously, i.e., don’t wait for the sound to finish before continuing the program.
   
            /*
    Together: SND_FILENAME | SND_ASYNC
        Play a song from a .wav file (SND_FILENAME)
        Play it in the background, letting your program continue (SND_ASYNC)
    */     
}



void stopMusic() {
    PlaySound(NULL, NULL, 0);
    printf("Music stopped.\n\n");
}

void nextSong() {

    Song *temp = current ? current->next : NULL; // If current is not NULL, meaning some song is currently selected, then we want to move to the next song.

    // keep looping while there are more songs in the list
    while (temp != NULL) {
        if (fileExists(temp->name)) {
            current = temp; // move to that song
            playCurrent();
            return;
        }
        // if file does not exist
        printf("Skipping missing file: %s\n", temp->name);
        temp = temp->next;
    }
    printf("End of playlist or no valid next song found.\n\n");
}

void prevSong() {
    Song *temp = current ? current->prev : NULL; // If a song is currently playing (current is not NULL), set temp to the previous song (current->prev).

    while (temp != NULL) {
        if (fileExists(temp->name)) {
            current = temp;
            playCurrent();
            return;
        }
        printf("Skipping missing file: %s\n", temp->name);
        temp = temp->prev;
    }
    printf("Start of playlist or no valid previous song found.\n\n");
}

void showPlaylist() {
    printf("\n--- Playlist ---\n");
    Song *temp = head;
    while (temp != NULL) {
        if (temp == current)
            printf("-> %s (Now Playing)\n", temp->name);
        else
            printf("   %s\n", temp->name);
        temp = temp->next;
    }
    printf("----------------\n\n");
}


// function used to load all saved songs from a file and add them back to the playist

void loadPlaylistFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("No playlist file found (%s).\n\n", filename);
        return;
    }

    char line[100]; // Abuffer to read each line of the file, one song per line 

    while (fgets(line, sizeof(line), file)) {
    
        //fgets -- reads a line of text from the file and stores it in line array
        // reads until new line 
        //sizeof(line) -- maximum size is reached


        // This line replaces the newline \n with a null terminator \0, removing the newline.
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            addSong(line);
        }
    }

    fclose(file);
    printf("Playlist loaded from %s\n\n", filename);
}


// responsible for freeing the memory when the playlist is no longer needed
void freePlaylist() {
    Song *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    head = tail = current = NULL;
}

void searchSong(char *name) {
    Song *temp = head;
    int found = 0;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Found: %s\n\n", temp->name);
            found = 1;
            break;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("Song not found: %s\n\n", name);
    }
}

void removeSong(char *name) {
    Song *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            if (temp->prev)
                temp->prev->next = temp->next;
            else
                head = temp->next;

            if (temp->next)
                temp->next->prev = temp->prev;
            else
                tail = temp->prev;

            if (current == temp)
                current = temp->next ? temp->next : temp->prev;

            free(temp);
            savePlaylistToFile("playlist.txt");
            printf("Removed: %s\n\n", name);
            return;
        }
        temp = temp->next;
    }
    printf("Song not found: %s\n\n", name);
}

void printMenu() {
    printf("======== MUSIC PLAYER MENU ========\n");
    printf("1. Add Song\n");
    printf("2. Choose and Play a Song\n");
    printf("3. Next Song\n");
    printf("4. Previous Song\n");
    printf("5. Show Playlist\n");
    printf("6. Exit\n");
    printf("7. Stop Music\n");
    printf("8. Search Song\n");
    printf("9. Remove Song\n");
    printf("===================================\n\n");
}


int main() {
    int choice;
    char songName[100];

    printf("\nWelcome to Your Music Player\n");
    loadPlaylistFromFile("playlist.txt");

    while (1) {
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter .wav filename: ");
                fgets(songName, sizeof(songName), stdin);
                songName[strcspn(songName, "\n")] = '\0';
                addSong(songName);
                break;

            case 2: {
                if (head == NULL) {
                    printf("Playlist is empty.\n\n");
                    break;
                }

                printf("\nAvailable Songs:\n");
                Song *temp = head;
                int index = 1;
                while (temp != NULL) {
                    printf("   %d. %s\n", index++, temp->name);
                    temp = temp->next;
                }
                printf("\n");

                int songChoice;
                printf("Enter song number to play: ");
                scanf("%d", &songChoice);
                getchar();

                temp = head;
                index = 1;
                while (temp != NULL && index < songChoice) {
                    temp = temp->next;
                    index++;
                }

                if (temp == NULL) {
                    printf("Invalid song number.\n\n");
                } else {
                    current = temp;
                    playCurrent();
                }
                break;
            }

            case 3:
                nextSong();
                break;

            case 4:
                prevSong();
                break;

            case 5:
                showPlaylist();
                break;

            case 6:
                printf("Exiting...\n");
                stopMusic();
                freePlaylist();
                return 0;

            case 7:
                stopMusic();
                break;

            case 8:
                printf("Enter song name to search: ");
                fgets(songName, sizeof(songName), stdin);
                songName[strcspn(songName, "\n")] = '\0';
                searchSong(songName);
                break;

            case 9:
                printf("Enter song name to remove: ");
                fgets(songName, sizeof(songName), stdin);
                songName[strcspn(songName, "\n")] = '\0';
                removeSong(songName);
                break;

            default:
                printf("Invalid choice.\n\n");
        }

        Sleep(300);  // small delay for better display
    }

    return 0;
}

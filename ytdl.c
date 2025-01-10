#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include <wchar.h>
#define MAX_OPTIONS 4
#define MAX_URL_LENGTH 1024
#define MAX_COMMAND_LENGTH 2048
#define MAX_PATH_LENGTH 2048
#define TOOL "curl"
#define TOOL2 "tar"
#define LIB1_PATH ".\\lib\\yt-dlp.exe"
#define LIB2_PATH ".\\lib\\ffmpeg-7.1-essentials_build\\bin\\ffmpeg.exe" // compile vedio

// Function prototypes
void check_library();                                                           // Check the  status of necessary libraries and tools
void install_ytdlp();                                                           // Install yt-dlp if it's not available
void install_ffmpeg();                                                          // Install ffmpeg if it's not available
int check_path_valid(const char *path);                                         // Check if a given path is valid
void setup_path();                                                              // Setup the path for downloads and save it
void display_menu(int selected_option);                                         // Display the main menu with selectable options
void display_settings_menu(int selected_option);                                // Display the settings menu
void pause();                                                                   // Pause the program and wait for user input
void clearConsole();                                                            // Clear the console screen
void mp3_download();                                                            // Handle MP3 download functionality
void mp4_download();                                                            // Handle MP4 download functionality
void saveDownloadPathToDatFile(const char *filename, const char *downloadPath); // Save the path to a dat file
void loadDownloadPathFromDatFile(const char *filename);                         // Load the path from the dat file
void create_folder();                                                           // Create folder for saving downloaded content
void toggle_create_folder();                                                    // Toggle the setting for folder creation
void sanitize_filename(char *filename);                                         // Sanitize the filename to remove special characters
int stateApp();                                                                 // Initialize the app state and check environment
int app();                                                                      // Main application loop

// Struct variable
typedef struct LibraryStatus
{
    char library[50]; // tool : curl, tar || library : yt-dlp, ffmpeg
    char status[10];
    char error_message[100];
} LibraryStatus;

typedef struct tools
{
    int error;
    char url[MAX_URL_LENGTH];
    char command[MAX_COMMAND_LENGTH];
    char directoryPath[MAX_PATH_LENGTH];
    int setting_status; // disabled, enabled
} tools;

tools ytDL;
LibraryStatus lstatus[5];

// Function implementations
void check_library()
{
    FILE *fp;
    char buffer[128];
    int idx = 0;

    // Check tools
    const char *tools[] = {TOOL, TOOL2};
    const char *tool_commands[] = {"curl --version", "tar --version"};
    for (int i = 0; i < 2; i++)
    {
        snprintf(lstatus[idx].library, sizeof(lstatus[idx].library), "tool : %-20s", tools[i]);
        if ((fp = popen(tool_commands[i], "r")) != NULL)
        {
            if (fgets(buffer, sizeof(buffer), fp) != NULL)
            {
                snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "pass");
            }
            else
            {
                snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "fail");
                snprintf(lstatus[idx].error_message, sizeof(lstatus[idx].error_message), "%s is not available. Please install %s.", tools[i], tools[i]);
            }
            pclose(fp);
        }
        else
        {
            snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "fail");
            snprintf(lstatus[idx].error_message, sizeof(lstatus[idx].error_message), "%s is not available. Please install %s.", tools[i], tools[i]);
        }
        idx++;
    }

    // Check libraries
    const char *libs[] = {LIB1_PATH, LIB2_PATH};
    for (int i = 0; i < 2; i++)
    {
        snprintf(lstatus[idx].library, sizeof(lstatus[idx].library), "lib  : %-20s", libs[i]);
        if (access(libs[i], F_OK) != -1)
        {
            snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "pass");
        }
        else
        {
            snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "fail");
            snprintf(lstatus[idx].error_message, sizeof(lstatus[idx].error_message), "%s is not available. Please make sure the file exists.", libs[i]);
        }
        idx++;
    }

    // Check path
    snprintf(lstatus[idx].library, sizeof(lstatus[idx].library), "app  : %-20s", "config.dat");
    if (check_path_valid("config.dat"))
    {
        snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "pass");
    }
    else
    {
        snprintf(lstatus[idx].status, sizeof(lstatus[idx].status), "fail");
        snprintf(lstatus[idx].error_message, sizeof(lstatus[idx].error_message), "config.dat is not available. Please make sure the file exists.");
    }

    // Print results
    for (int i = 0; i <= idx; i++)
    {
        printf("%s: %s\n", lstatus[i].library, lstatus[i].status);
    }

    // Print errors at the end
    printf("\n");
    for (int i = 0; i <= idx; i++)
    {
        if (strcmp(lstatus[i].status, "fail") == 0)
        {
            printf("error : %s\n", lstatus[i].error_message);
        }
    }
}

void install_ytdlp()
{
    char ytDL_URL[MAX_URL_LENGTH] = "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe",
         ytDL_PATH[MAX_PATH_LENGTH] = "./lib/yt-dlp.exe";
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "curl -L %s -o %s", ytDL_URL, ytDL_PATH);

    printf("Downloading yt-dlp...\n");
    int result = system(command);
    if (result == 0)
    {
        printf("yt-dlp installed successfully.\n");
    }
    else
    {
        printf("Failed to install yt-dlp.\n");
    }
}

void install_ffmpeg()
{
    char FFMPEG_URL[MAX_URL_LENGTH] = "https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip",
         FFMPEG_ZIP_PATH[MAX_PATH_LENGTH] = "./lib/ffmpeg.zip",
         *FFMPEG_EXTRACT_PATH = "./lib/";
    char command[MAX_COMMAND_LENGTH];
    // Download ffmpeg zip file
    snprintf(command, sizeof(command), "curl -L %s -o %s", FFMPEG_URL, FFMPEG_ZIP_PATH);
    printf("Downloading ffmpeg...\n");
    int result = system(command);
    if (result != 0)
    {
        printf("Failed to download ffmpeg.\n");
        return;
    }

    // Extract ffmpeg zip file
    snprintf(command, sizeof(command), "tar -xvf %s -C %s", FFMPEG_ZIP_PATH, FFMPEG_EXTRACT_PATH);
    printf("Extracting ffmpeg...\n");
    result = system(command);
    if (result == 0)
    {
        printf("ffmpeg installed successfully.\n");
    }
    else
    {
        printf("Failed to extract ffmpeg.\n");
    }
}

int check_path_valid(const char *path)
{
    return access(path, F_OK) != -1;
}
void saveDownloadPathToDatFile(const char *filename, const char *downloadPath)
{
    FILE *file = fopen(filename, "wb"); // Open the file in write binary mode
    if (file == NULL)
    {
        printf("Cannot open the file\n");
        return;
    }

    size_t path_length = strlen(downloadPath) + 1;         // Include null terminator
    fwrite(&path_length, sizeof(path_length), 1, file);    // Write the length of the downloadPath
    fwrite(downloadPath, sizeof(char), path_length, file); // Write the downloadPath in binary format

    fclose(file);
    printf("Path has been saved\n");
}
void loadDownloadPathFromDatFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Cannot open the file\n");
        return;
    }

    size_t path_length;
    fread(&path_length, sizeof(path_length), 1, file);

    char *downloadPath = (char *)malloc(path_length);
    if (downloadPath == NULL)
    {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }

    fread(downloadPath, sizeof(char), path_length, file);
    printf("The saved download path is: %s\n", downloadPath);

    strcpy(ytDL.directoryPath, downloadPath); // Correctly update the global structure
    free(downloadPath);
    fclose(file);
    printf("%s\n", ytDL.directoryPath);
}

void setup_path()
{

    printf("Enter the directory path for downloads: ");
    fgets(ytDL.directoryPath, MAX_PATH_LENGTH, stdin);
    ytDL.directoryPath[strcspn(ytDL.directoryPath, "\n")] = 0;

    if (!check_path_valid(ytDL.directoryPath))
    {
        printf("The path is invalid. Please enter a valid path.\n");
        setup_path();
    }
    saveDownloadPathToDatFile("config.dat", ytDL.directoryPath);
    loadDownloadPathFromDatFile("config.dat");
}
void display_menu(int selected_option)
{
    clearConsole(); // Clear the console before displaying the menu

    printf("\n\t*------------------------------*\n");
    printf("\t\tYouTube Downloader\n");
    printf("\t*------------------------------*\n");

    // Display the main menu options with titles
    for (int i = 0; i < 4; i++)
    {
        if (i == selected_option)
        {
            switch (i)
            {
            case 0:
                printf("\t\t[*] Download MP4\n");
                break;
            case 1:
                printf("\t\t[*] Download MP3\n");
                break;
            case 2:
                printf("\t\t[*] Setting\n");
                break;
            default:
                printf("\t\t[*] Exit\n");
                break;
            }
        }
        else
        {
            switch (i)
            {
            case 0:
                printf("\t\t[ ] Download MP4\n");
                break;
            case 1:
                printf("\t\t[ ] Download MP3\n");
                break;
            case 2:
                printf("\t\t[ ] Setting\n");
                break;
            default:
                printf("\t\t[ ] Exit\n");
                break;
            }
        }
    }

    printf("\nUse 'w' to move up, 's' to move down, and 'Enter' to select an option.\n");
}

void display_settings_menu(int selected_option)
{
    clearConsole(); // Clear the console before displaying the settings menu
    printf("\n");
    printf("\t\t[#]-- Setting -->\n");
    printf("\n");

    // Display the settings options
    for (int i = 0; i < 4; i++)
    {
        if (i == selected_option)
        {
            if (i == 0)
            {
                printf("\t\t[*] File Directory\n");
                printf("\t\t\t< %s >\n", ytDL.directoryPath);
            }
            else if (i == 1)
            {
                printf("\t\t[*] Create Folder\n");
                printf("\t\t\t< %s >\n", ytDL.setting_status ? "Enabled" : "Disabled");
            }
            else if (i == 2)
            {
                printf("\t\t[*] Mini Manual\n");
            }
            else
            {
                printf("\n\t\t[*] Back\n");
            }
        }
        else
        {
            if (i == 0)
            {
                printf("\t\t[ ] File Directory\n");
                printf("\t\t\t< %s >\n", ytDL.directoryPath);
            }
            else if (i == 1)
            {
                printf("\t\t[ ] Create Folder\n");
                printf("\t\t\t< %s >\n", ytDL.setting_status ? "Enabled" : "Disabled"); //<enable/disable>
            }
            else if (i == 2)
            {
                printf("\t\t[ ] Mini Manual\n");
            }
            else
            {
                printf("\n\t\t[ ] Back\n");
            }
        }
    }

    printf("\nUse 'w' to move up, 's' to move down, 'Enter' to select an option\n");
}

void pause()
{
    printf("\n\nPress Enter to continue...");
    getch();
}

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void sanitize_filename(char *filename)
{
    for (int i = 0; filename[i] != '\0'; i++)
    {
        if (
            filename[i] == '\\' || filename[i] == '/' || filename[i] == ':' ||
            filename[i] == '*' || filename[i] == '?' || filename[i] == '\"' ||
            filename[i] == '<' || filename[i] == '>' || filename[i] == '|' ||
            filename[i] == '(' || filename[i] == ')' || filename[i] == '\'')
        {
            filename[i] = '_'; // Replace special characters with '_'
        }
    }
}

void mp4_download()
{
    create_folder();
    clearConsole();
    char title[1048], command[MAX_COMMAND_LENGTH], format_id[128];
    printf("\n");
    printf("\n\t\tYouTube MP4 Downloader\n");
    printf("\t*-------------------------------------*\n");
    printf("\t warning : if you enter Playlist url it will download all of them\n");
    printf("\t enter 0 : back to main menu\n");
    printf("\t > right click for paste URL\n");
    printf("\t*-------------------------------------*\n");
    printf("Enter URL: ");
    fgets(ytDL.url, MAX_URL_LENGTH, stdin);
    ytDL.url[strcspn(ytDL.url, "\n")] = 0;
    if (strcmp(ytDL.url, "0") == 0) // Return to main menu if user inputs 0
    {
        return;
    }
    printf("\t#-------------------------------------#\n");
    clearConsole();
    snprintf(command, sizeof(command),
             "%s --get-title \"%s\" --ffmpeg-location \"%s\"",
             LIB1_PATH, ytDL.url, LIB2_PATH);
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Failed processing ");
        return;
    }
    if (fgets(title, sizeof(title), fp) != NULL)
    {
        title[strcspn(title, "\n")] = 0; // delete newline
        printf("Title: %s\n", title);    // show up title
        sanitize_filename(title);
    }
    printf("ID  EXT   RESOLUTION  FPS      FILESIZE     TBR   PROTO  VCODEC          VBR   ACODEC            MORE INFO\n");
    printf("___________________________________________________________________________________________________________________\n");
    snprintf(command, sizeof(command),
             "%s -F \"%s\" | findstr \"vp\"", LIB1_PATH, ytDL.url);
    fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Failed to retrieve format list");
        return;
    }

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp))
    {
        printf("%s", buffer);
    }
    pclose(fp);

    printf("Enter 0 : back to main menu\n");
    printf("\n");
    printf("Enter the format ID: ");
    fgets(format_id, sizeof(format_id), stdin);
    format_id[strcspn(format_id, "\n")] = 0;

    if (strcmp(format_id, "0") == 0)
    {
        return;
    }
    snprintf(command, sizeof(command),
             "%s -f \"%s+140\" --ffmpeg-location \"%s\" --merge-output-format mp4 -o \"%s\\%s.%%(ext)s\" \"%s\"",
             LIB1_PATH, format_id, LIB2_PATH, ytDL.directoryPath, title, ytDL.url);
    if (system(command) == 0)
    {
        printf("\n\t*---------------------------------*\n");
        printf("\t\tDownload successful\n");
        printf("\t*---------------------------------*\n");
    }
    else
    {
        printf("\n\t*---------------------------------*\n");
        printf("\t\tDownload failed\n");
        printf("\t*---------------------------------*\n");
    }
}

void mp3_download()
{
    create_folder();
    clearConsole();
    char title[1048];
    char command[MAX_COMMAND_LENGTH];
    printf("\n\t\tYouTube MP3 Downloader\n");
    printf("\t*-------------------------------------*\n");
    printf("\t warning : if you enter Playlist url it will download all of them\n");
    printf("\t enter 0 : back to main menu\n");
    printf("\t > right click for paste URL\n");
    printf("\t*-------------------------------------*\n");
    printf("Enter URL: ");
    fgets(ytDL.url, MAX_URL_LENGTH, stdin);
    ytDL.url[strcspn(ytDL.url, "\n")] = 0;
    if (strcmp(ytDL.url, "0") == 0) // Return to main menu if user inputs 0
    {
        return;
    }

    snprintf(command, sizeof(command),
             "%s --get-title \"%s\" --ffmpeg-location \"%s\"",
             LIB1_PATH, ytDL.url, LIB2_PATH);
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Failed processing ");
        return;
    }

    if (fgets(title, sizeof(title), fp) != NULL)
    {
        title[strcspn(title, "\n")] = 0; // delete newline
        printf("Title: %s\n", title);    // show up title
        sanitize_filename(title);
    }
    else
    {
        printf("Failed to fetch title.\n");
    }

    printf("\t#-------------------------------------#\n");

    const char *bitrates[4][2] = {{"TBR: 33k\t| 760.83KiB | opus", "600"},
                                  {"TBR: 48k\t|  1.11MiB  | opus", "249"},
                                  {"TBR: 63k\t|  1.43MiB  | opus", "250"},
                                  {"TBR: 124k\t|  2.82MiB  | opus", "251"}};

    int selected_bitrate = 0, row, colume;
    while (1)
    {
        clearConsole();
        printf("\n");
        printf("Title: %s\n", title);
        printf("\t#-------------------------------------#\n");
        printf("\t\tSelect Bitrate:\n");
        for (int i = 0; i < 4; i++)
        {
            if (i == selected_bitrate)
            {
                printf("\t[*] %s\n", bitrates[i][0]);
            }
            else
            {
                printf("\t[ ] %s\n", bitrates[i][0]);
            }
        }

        printf("\nUse 'w' to move up, 's' to move down, and 'Enter' to select an option.\n");

        char input = getch(); // Get user input
        if (input == 'w' || input == 'W')
        {
            selected_bitrate = (selected_bitrate - 1 + 4) % 4; // Navigate up
        }
        else if (input == 's' || input == 'S')
        {
            selected_bitrate = (selected_bitrate + 1) % 4; // Navigate down
        }
        else if (input == '\r')
        {
            printf("\nYou have selected bitrate: %s\n", bitrates[selected_bitrate][0]);
            switch (selected_bitrate)
            {
            case 0:
                row = 0;
                colume = 1;
                break;
            case 1:
                row = 1;
                colume = 1;
                break;
            case 2:
                row = 2;
                colume = 1;
                break;
            case 3:
                row = 3;
                colume = 1;
                break;
            }
            snprintf(command, sizeof(command),
                     "%s \"%s\" -f %s -o \"%s\\%s.%%(ext)s\"",
                     LIB1_PATH, ytDL.url, bitrates[row][colume], ytDL.directoryPath, title);
            if (system(command) == 0)
            {
                char input_file[MAX_PATH_LENGTH];
                char output_file[MAX_PATH_LENGTH];
                snprintf(input_file, sizeof(input_file), "%s\\%s.webm", ytDL.directoryPath, title);
                snprintf(output_file, sizeof(output_file), "%s\\%s.mp3", ytDL.directoryPath, title);

                snprintf(command, sizeof(command),
                         "%s -i \"%s\" -vn -ar 44100 -ac 2 -b:a 192k \"%s\"",
                         LIB2_PATH, input_file, output_file); // ffmpeg command to convert webm to mp3

                if (system(command) == 0)
                {
                    snprintf(command, sizeof(command),
                             "del \"%s\"", input_file); // Delete the webm file
                    system(command);
                    printf("\n\t*---------------------------------*\n");
                    printf("\t\tDownload and conversion successful\n");
                    printf("\t*---------------------------------*\n");
                }
                else
                {
                    printf("\n\t*---------------------------------*\n");
                    printf("\t\tConversion failed\n");
                    printf("\t*---------------------------------*\n");
                }
            }
            else
            {
                printf("\n\t*---------------------------------*\n");
                printf("\t\tDownload Failed\n");
                printf("\t*---------------------------------*\n");
            }
            break;
        }
    }
}

// Initialize the app state and check environment
int stateApp()
{
    printf("State 1: Checking environment...\n");
    check_library();

    int allPassed = 1;
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(lstatus[i].status, "fail") == 0)
        {
            allPassed = 0;
            break;
        }
    }

    if (allPassed)
    {
        loadDownloadPathFromDatFile("config.dat");
        printf("%s\n", ytDL.directoryPath);
        printf("All checks passed in State 1. Skipping subsequent states.\n");
        app();
        return 0;
    }

    printf("State 2: Downloading required libraries...\n");
    system("mkdir lib");
    if (strcmp(lstatus[2].status, "fail") == 0)
    {
        install_ytdlp();
    }
    if (strcmp(lstatus[3].status, "fail") == 0)
    {
        install_ffmpeg();
    }

    printf("State 3: Setting up path...\n");
    if (strcmp(lstatus[4].status, "fail") == 0)
    {
        setup_path();
    }

    printf("State 4: Rechecking environment...\n");

    // Load the path from the config file
    char downloadPath[MAX_PATH_LENGTH];
    loadDownloadPathFromDatFile("config.dat");
    check_library();

    allPassed = 1;
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(lstatus[i].status, "fail") == 0)
        {
            printf("Error in State 4: %s\n", lstatus[i].error_message);
            allPassed = 0;
        }
    }

    if (!allPassed)
    {
        printf("State 4 failed. Exiting program.\n");
        return -1;
    }

    printf("State 5: Starting program...\n");
    app();
}

void create_folder()
{
    loadDownloadPathFromDatFile("config.dat");
    char folder_path[MAX_PATH_LENGTH];
    if (ytDL.setting_status)
    {
        snprintf(folder_path, sizeof(folder_path), "%s/playlist", ytDL.directoryPath);

        // Check if the folder exists, and create it if it doesn't
        if (access(folder_path, F_OK) == -1)
        {
            if (mkdir(folder_path) == 0) // Remove the 0777 argument
            {
                printf("Playlist folder created at %s\n", folder_path);
            }
            else
            {
                printf("Failed to create playlist folder.\n");
            }
        }
        else
        {
            printf("Playlist folder already exists at %s\n", folder_path);
        }
        strcpy(ytDL.directoryPath, folder_path); // Update the download path to the new folder
    }
    else
    {
        loadDownloadPathFromDatFile("config.dat");
        printf("Folder creation is disabled.\n");
    }
}
void toggle_create_folder()
{
    // Toggle the create folder setting
    ytDL.setting_status = !ytDL.setting_status;

    // Print the new setting status
    printf("Create Folder is now %s\n", ytDL.setting_status ? "Enabled" : "Disabled");
}
wchar_t input = L'ก';
// Main application loop
int app()
{
    int selected_option = 0; // Start with the first option selected
    int in_settings = 0;     // Flag to track if we are in the settings menu
    char input;
    do
    {
        clearConsole();

        if (in_settings)
        {
            display_settings_menu(selected_option); // Settings menu
        }
        else
        {
            display_menu(selected_option); // Main menu
        }

        input = getch();

        if (input == 'w' || input == 'W')
        {
            selected_option = (selected_option - 1 + (in_settings ? 4 : 4)) % (in_settings ? 4 : 4); // Move up, wrap around
        }
        else if (input == 's' || input == 'S')
        {
            selected_option = (selected_option + 1) % (in_settings ? 4 : 4); // Move down, wrap around to top
        }

        if (input == '\r')
        {
            if (in_settings)
            {
                switch (selected_option)
                {
                case 0:
                    setup_path();
                    break;
                case 1:
                    toggle_create_folder();
                    break;
                case 2:
                    printf("\n\n File Directory    : Set up the file path location to save the downloaded files.\n");
                    printf("                   The path location is not reset when you open a new program.\n");
                    printf("                   <{Path}> is the present path loaction.\n\n");

                    printf(" Create Folder     : Create a playlist folder after downloading a file.\n");
                    printf("                   **The status is reset each time you open a new program.\n");
                    printf("                   <Enable>  This will cause a new folder to be created every time a file is downloaded to store that file.\n");
                    printf("                             However, if the Playlist folder already exists, the file will be stored there instead.\n");
                    printf("                   <Disable>  No folder will be created, and the file will be stored in the default location.\n");
                    pause();
                    break;
                case 3:
                    in_settings = 0; // back
                    break;
                }
            }
            else
            {
                switch (selected_option)
                {
                case 0:
                    mp4_download();
                    break;
                case 1:
                    mp3_download();
                    break;
                case 2:
                    in_settings = 1; // Setting
                    break;
                case 3:
                    selected_option = '3';
                    printf("\nExit...\n");

                    break;
                }
            }
        }

    } while (selected_option != '3');
    return 0;
}

int main(int argc, char const *argv[])
{
    stateApp();

    return 0;
}

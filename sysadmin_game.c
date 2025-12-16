#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(x) Sleep(x)
#else
    #include <unistd.h>
    #define SLEEP_MS(x) usleep((x)*1000)
#endif

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"
#define CLEAR_SCR "\x1b[1;1H\x1b[2J"

#define MAX_FILES 16
#define NAME_LEN 32
#define CONTENT_LEN 256
#define SAVE_FILE "sysadmin_save.dat"

typedef struct {
    char name[NAME_LEN];
    int size;
    int is_virus; 
    char content[CONTENT_LEN]; 
} FileEntry;

typedef struct Directory {
    char name[NAME_LEN];
    FileEntry files[MAX_FILES];
    int file_count;
    struct Directory *parent;
    struct Directory *subdirs[8];
    int subdir_count;
} Directory;

void save_game(int lives) {
    FILE *fp = fopen(SAVE_FILE, "w");
    if (fp == NULL) {
        printf(C_RED "Error: Unable to save game data.\n" C_RESET);
        return;
    }
    fprintf(fp, "%d", lives);
    fclose(fp);
    printf(C_GREEN "Game progress saved successfully to '%s'.\n" C_RESET, SAVE_FILE);
}

int load_game() {
    FILE *fp = fopen(SAVE_FILE, "r");
    int lives = 3; 
    if (fp != NULL) {
        if (fscanf(fp, "%d", &lives) != 1) {
            lives = 3;
        }
        fclose(fp);
        printf(C_CYAN "Save file found. Resuming with %d lives.\n" C_RESET, lives);
        SLEEP_MS(2500);
    }
    return lives;
}

Directory* new_dir(const char *name, Directory *parent) {
    Directory *d = malloc(sizeof(Directory));
    strncpy(d->name, name, NAME_LEN);
    d->file_count = 0;
    d->subdir_count = 0;
    d->parent = parent;
    return d;
}

void add_file(Directory *d, const char *fname, int size, int is_virus, const char *content) {
    if (d->file_count >= MAX_FILES) return;
    FileEntry *f = &d->files[d->file_count++];
    strncpy(f->name, fname, NAME_LEN);
    strncpy(f->content, content, CONTENT_LEN);
    f->size = size; 
    f->is_virus = is_virus;
}

void print_header(int lives) {
    printf(C_BLUE "\n========================================\n" C_RESET);
    printf(" USER: " C_CYAN "root" C_RESET " | LIVES: ");
    for(int i=0; i<lives; i++) printf(C_RED "<3 " C_RESET);
    printf("\n" C_BLUE "========================================\n" C_RESET);
}

void ls(Directory *d) {
    printf("Contents of " C_YELLOW "%s" C_RESET ":\n", d->name);
    
    for (int i=0;i<d->subdir_count;i++) {
        printf("  " C_BLUE "[%s]" C_RESET "\n", d->subdirs[i]->name);
    }
    for (int i=0;i<d->file_count;i++) {
        printf("  %s \t" C_CYAN "(%d KB)" C_RESET "\n", d->files[i].name, d->files[i].size);
    }
    if (d->file_count == 0 && d->subdir_count == 0) {
        printf("  (empty)\n");
    }
}

void cat_file(Directory *d, const char *fname) {
    for (int i=0;i<d->file_count;i++) {
        if (strcmp(d->files[i].name, fname) == 0) {
            printf("\n--- START OF FILE ---\n");
            printf("%s\n", d->files[i].content);
            printf("--- END OF FILE ---\n");
            
            if(d->files[i].is_virus) {
                printf(C_RED "\n[SYSTEM WARNING] Integrity error while reading block!\n" C_RESET);
            }
            return;
        }
    }
    printf(C_RED "File not found.\n" C_RESET);
}

void scan_file(Directory *d, const char *fname) {
    for (int i=0;i<d->file_count;i++) {
        if (strcmp(d->files[i].name, fname) == 0) {
            printf("Scanning %s...", fname);
            fflush(stdout); 
            
            for(int k=0; k<3; k++) {
                SLEEP_MS(300); 
                printf("."); 
                fflush(stdout);
            }
            
            if (d->files[i].is_virus) {
                printf("\nRESULT: " C_RED "[DANGER] MALWARE SIGNATURE DETECTED!" C_RESET "\n");
                printf("Action: Use 'rm %s' to remove threat.\n", fname);
            } else {
                printf("\nRESULT: " C_GREEN "[SAFE] No threats found." C_RESET "\n");
            }
            return;
        }
    }
    printf(C_RED "File not found to scan.\n" C_RESET);
}

Directory* cd(Directory *current, const char *arg) {
    if (strcmp(arg, "..") == 0) return current->parent ? current->parent : current;
    for (int i=0;i<current->subdir_count;i++) {
        if (strcmp(current->subdirs[i]->name, arg) == 0) return current->subdirs[i];
    }
    printf(C_RED "Directory not found: %s\n" C_RESET, arg);
    return current;
}

int rm_file(Directory *d, const char *fname) {
    for (int i=0;i<d->file_count;i++) {
        if (strcmp(d->files[i].name, fname) == 0) {
            for (int j=i;j<d->file_count-1;j++) d->files[j]=d->files[j+1];
            d->file_count--;
            return 1;
        }
    }
    return 0;
}

void print_help() {
    printf("\n" C_YELLOW "--- COMMAND MANUAL ---" C_RESET "\n");
    printf(" ls          : List content (Folders in BLUE)\n");
    printf(" cd <dir>    : Change directory (use '..' to go back)\n");
    printf(" cat <file>  : Read file content\n");
    printf(" scan <file> : Analyze file for viruses\n");
    printf(" rm <file>   : Delete a file\n");
    printf(" save        : Save current progress (Lives)\n");
    printf(" clear       : Clear terminal screen\n");
    printf(" help        : Show this menu\n");
    printf(" quit        : Exit game\n");
}

int main(){
    Directory *root = new_dir("root", NULL);
    Directory *etc = new_dir("etc", root);
    Directory *home = new_dir("home", root);
    Directory *downloads = new_dir("downloads", home);
    
    root->subdirs[root->subdir_count++] = etc;
    root->subdirs[root->subdir_count++] = home;
    home->subdirs[home->subdir_count++] = downloads;

    add_file(root, "readme.txt", 2, 0, "SERVER ALERT: Unusual traffic detected in /home/downloads.");
    add_file(etc, "config.sys", 5, 0, "MAX_USERS=100\nFIREWALL=ON");
    add_file(home, "note.txt", 1, 0, "Remember to clear the Downloads folder.");
    
    add_file(downloads, "free_ram_optimizer.exe", 850, 1, "0xDEADBEEF MALICIOUS CODE...");
    add_file(downloads, "funny_cat.jpg", 150, 0, "[Image data not displayed]");

    Directory *cur = root;
    char command[128];
    int lives = load_game();

    printf(CLEAR_SCR); 
    printf(C_GREEN);
    printf(" ::::::::  :::   :::  ::::::::      :::     :::::::::  ::::    ::::  ::::::::::: ::::    ::: \n");
    printf(":+:    :+: :+:   :+: :+:    :+:   :+: :+:   :+:    :+: +:+:+: :+:+:+     :+:     :+:+:   :+: \n");
    printf("+:+         +:+ +:+  +:+         +:+   +:+  +:+    +:+ +:+ +:+:+ +:+     +:+     :+:+:+  +:+ \n");
    printf("+#++:++#++   +#++:   +#++:++#++ +#++:++#++: +#+    +:+ +#+  +:+  +#+     +#+     +#+ +:+ +#+ \n");
    printf("       +#+    +#+           +#+ +#+     +#+ +#+    +#+ +#+       +#+     +#+     +#+  +#+#+#\n");
    printf("#+#    #+#    #+#    #+#    #+# #+#     #+# #+#    #+# #+#       #+#     #+#     #+#   #+#+#\n");
    printf(" ########     ###     ########  ###     ### #########  ###       ### ########### ###    #### \n");
    printf(C_RESET "\n");
    printf("Server compromised. Locate and delete the malware.\n");
    printf("Hint: Check 'readme.txt' first.\n");
    printf("Type 'help' for commands.\n");

    while (lives>0) {
        print_header(lives);
        printf("[%s]$ ", cur->name);
        
        if (!fgets(command, sizeof(command), stdin)) break;
        command[strcspn(command,"\n")] = 0; 

        if (strcmp(command, "clear") == 0 || strcmp(command, "cls") == 0) {
            printf(CLEAR_SCR);
            continue; 
        }
        else if (strcmp(command, "help") == 0) {
            print_help();
        }
        else if (strcmp(command, "save") == 0) {
            save_game(lives);
        }
        else if (strncmp(command,"ls",2)==0) {
            ls(cur);
        }
        else if (strncmp(command,"cd ",3)==0) {
            char arg[64]; sscanf(command+3, "%63s", arg);
            cur = cd(cur, arg);
        }
        else if (strncmp(command,"cat ",4)==0) {
            char arg[64]; sscanf(command+4, "%63s", arg);
            cat_file(cur, arg);
        }
        else if (strncmp(command,"scan ",5)==0) {
            char arg[64]; sscanf(command+5, "%63s", arg);
            scan_file(cur, arg);
        }
        else if (strncmp(command,"rm ",3)==0) {
            char arg[64]; sscanf(command+3, "%63s", arg);
            
            int is_virus_target = 0;
            for(int i=0; i<cur->file_count; i++){
                if(strcmp(cur->files[i].name, arg)==0 && cur->files[i].is_virus) 
                    is_virus_target = 1;
            }

            if (rm_file(cur, arg)) {
                if(is_virus_target) {
                    printf(CLEAR_SCR);
                    printf(C_GREEN "\n>>> THREAT ELIMINATED <<<\n" C_RESET);
                    printf("Server integrity restored.\n");
                    printf("Good job, Admin!\n");
                    remove(SAVE_FILE);
                    break; 
                } else {
                    printf(C_RED "CRITICAL ERROR: You deleted a system file!\n" C_RESET);
                    lives--;
                }
            } else {
                printf(C_RED "File not found: %s\n" C_RESET, arg);
            }
        }
        else if (strcmp(command,"quit")==0) break;
        else {
            printf("Unknown command. Type 'help'.\n");
        }
    }

    if (lives==0) {
        printf(C_RED "\nSYSTEM FAILURE. Connection Lost.\n" C_RESET);
        remove(SAVE_FILE);
    }
    
    return 0;
}
#include "dir.h"

char** file_contents = NULL;

long* size_of_array;

int length_of_array;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int length = 0;

char path[1024];

char beginning[1024];

char* new_path;


void *listdir(void *arg) {
    pthread_mutex_lock(&mutex);
	char* directory = (char*) arg;
    //struct stat file;
    FILE *fptr;
    char buffer[1024];
	sprintf (buffer, "%s", path);
	sprintf (buffer + strlen(buffer), "/%s", directory);
    DIR *dir = opendir(buffer);
    struct dirent *entry;
	int num;
    // get dir pass and check the type
    printf("%s\n", buffer);
    
    while ((entry = readdir(dir)) != NULL) {
    	printf("%s\n", path);
        if (entry->d_type == DT_LNK) {
        	continue;
        } else if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%s\n", path);
            new_path = entry->d_name;
            sprintf (path + strlen(path), "/%s", directory);
            pthread_mutex_unlock(&mutex);
            listdir((void*)new_path);
        } else {
        	printf("FILE FOUND\n");
            sprintf (buffer + strlen(buffer), "/%s", entry->d_name);
        	fptr = fopen(buffer, "rb");
            num = atoi(&entry->d_name[0]);
            if (num >= length)
            {
                length = num;
                file_contents = realloc(file_contents, sizeof(char*) * length);
            }
        	//if string == num
        	//length = num
        	//make sure to check pathing
            fseek(fptr, 0L, SEEK_END);
            int sz = ftell(fptr);
            rewind(fptr);
        	fread(file_contents[num-1],sizeof(char), sz, fptr);
            buffer[strlen(buffer) - strlen(entry->d_name) - 1] = '\0';
        	fclose(fptr);
            pthread_mutex_unlock(&mutex);
        }
    }
    
    //int file_size = file.st_size;
    closedir(dir);
    sprintf (path, "%s", beginning);
    return 0;

} 


int main(int argc, char **argv) {
	int threads = 0;
	DIR* dir = opendir(argv[1]);
	sprintf (beginning, "%s", argv[1]);
    sprintf (path, "%s", argv[1]);
    struct dirent *entry;
    char** array_of_files = NULL;
    //FILE *fptr;
	
	
	while((entry = readdir(dir)) != NULL) {
		if(entry->d_type == DT_LNK) {
			continue;
		} else {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            threads += 1;
            array_of_files = realloc(array_of_files, sizeof(char*) * threads);
            array_of_files[threads-1] = entry->d_name;
		}
	}
	
	pthread_t tids[threads];
	
	//starting threads 
	for (unsigned long i = 0; i < threads; ++i) {
		int err = pthread_create(&tids[i], NULL, listdir, (void*) array_of_files[i]);
		if (err != 0) {
			fprintf(stderr, "Couldn't create thread\n");
			return -1;
		}
	}
	
	//allowing the threads to wait for one another
	for(int i = 0; i < threads; ++i) {
		pthread_join(tids[i], NULL);
	}
	
    

	for(int i = 0; i < length_of_array; ++i) {
		//fptr = fopen(argv[2], "wb");
		//fwrite(file_contents[i], sizeof(char), size_of_array[i], fptr);
		break;
		
	}
	
	//writing to file
	free(array_of_files);
    free(dir);

    for (int i = 0; i < length; ++i) {
        printf(file_contents[i]);
        free(file_contents[i]);
    }
	
	free(file_contents);
	//fclose(fptr);
    
    
    return 0;
}


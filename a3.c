#include "dir.h"

char** file_contents = NULL;

long* size_of_array;

int length_of_array;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int length = 0;

DIR *dir;


void *listdir(void *arg) {
	pthread_mutex_lock(&mutex);
	char* directory = (char*) arg;
    //struct stat file;
    FILE *fptr;
    struct dirent *entry;
	chdir();
   // int num;
    printf("Works here\n");
    printf("%p\n", dir);
    // get dir pass and check the type
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_LNK) {
        	continue;
        } else if (entry->d_type == DT_DIR) {
        	printf("Works here\n");
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
			pthread_mutex_unlock(&mutex);
			printf("%s\n", path);
            listdir((void*)path);
        } else {
        	
        	
        	
        	fptr = fopen(entry->d_name, "rb");
        	//if string == num
        	//length = num
        	//make sure to check pathing
        	fread(file_contents,sizeof(char), length, fptr);
        	fclose(fptr);
        }
    }
    
    //int file_size = file.st_size;
    
    pthread_mutex_lock(&mutex);
    
    //if (length < (num + 1)) {
    //	length = num + 1;
    
    
    file_contents = realloc(file_contents, sizeof(char*) * length);
    
    pthread_mutex_unlock(&mutex);
    
    closedir(dir);
    
    return 0;

} 


int main(int argc, char **argv) {
	int threads = 0;
	dir = opendir(argv[1]);
    struct dirent *entry;
    char** array_of_files = NULL;
    FILE *fptr;
	
	
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
		fptr = fopen(argv[2], "wb");
		fwrite(file_contents[i], sizeof(char), size_of_array[i], fptr);
		
	}
	
	//writing to file
	for(int i = 0; i < threads; ++i) {
		free(file_contents[i]);
		free(array_of_files[i]);
	}
	
	free(file_contents);
	free(array_of_files);
	fclose(fptr);
    
    
    return 0;
}














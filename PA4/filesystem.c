#include <stdio.h> 
#include <stdlib.h>  
#include <string.h>

#define NUM_PAGES 32
#define FUTURE_SEEK_AMOUNT 100

struct main_memory_page{
	int process_number;
    int virtual_page_number;
    int offset;
	int dirty_bit;
    int reference_bit;
    int last_used_time_unit;
};

int get_physical_page_index(struct main_memory_page pages[NUM_PAGES], int process_number, __uint16_t virtual_page_number, __uint16_t offset){
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].process_number == process_number && pages[i].virtual_page_number == virtual_page_number){
            return i;
        }
    }
    return -1;
}

int get_replacement_random(struct main_memory_page pages[NUM_PAGES], int process_number, __uint16_t virtual_page_number, __uint16_t offset){
    return rand() % NUM_PAGES;
}

int fifo_index = 0;
int get_replacement_fifo(struct main_memory_page pages[NUM_PAGES], int process_number, __uint16_t virtual_page_number, __uint16_t offset){
    int result = fifo_index;
    fifo_index++;
    fifo_index = fifo_index % 32;
    return result;
}

int get_replacement_lru(struct main_memory_page pages[NUM_PAGES], int process_number, __uint16_t virtual_page_number, __uint16_t offset){
    int lru_min = 9999999;
    int lru_index;
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].last_used_time_unit < lru_min){
            lru_min = pages[i].last_used_time_unit;
            lru_index = i;
        }
    }
    return lru_index;
}

int get_replacement_per(struct main_memory_page pages[NUM_PAGES], int process_number, __uint16_t virtual_page_number, __uint16_t offset){
    
    // First check for unused page
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].process_number == -1){
            return i;
        }
    }

    // Page where reference bit is 0 and dirty bit is 0
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].reference_bit == 0 && pages[i].dirty_bit == 0){
            return i;
        }
    }

    // Page where reference bit is 0 and dirty bit is 1
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].reference_bit == 0 && pages[i].dirty_bit == 1){
            return i;
        }
    }

    // Page where reference bit is 1 and dirty bit is 0
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].reference_bit == 1 && pages[i].dirty_bit == 0){
            return i;
        }
    }

    // Page where reference bit is 1 and dirty bit is 1
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].reference_bit == 1 && pages[i].dirty_bit == 1){
            return i;
        }
    }

    return 0;
}

int get_replacement_future(struct main_memory_page pages[NUM_PAGES], int process_number, __uint16_t virtual_page_number, __uint16_t offset,
    struct main_memory_page future_pages[], int num_future_pages) {
    
    int future_hit_count[NUM_PAGES];
    for(int page_num = 0; page_num < NUM_PAGES; page_num++){
        future_hit_count[page_num] = 0;
        for(int future_page_num = 0; future_page_num < num_future_pages; future_page_num++){
            if(pages[page_num].process_number == future_pages[future_page_num].process_number && 
            pages[page_num].virtual_page_number == future_pages[future_page_num].virtual_page_number){
                future_hit_count[page_num]++;
            }
        }
    }

    int min_hit_count = 9999999;
    int lru = 999999;
    int min_page_index;
    for(int page_num = 0; page_num < NUM_PAGES; page_num++){
        if(future_hit_count[page_num] < min_hit_count || (future_hit_count[page_num] == min_hit_count && pages[page_num].last_used_time_unit < lru)){
            min_hit_count = future_hit_count[page_num];
            lru = pages[page_num].last_used_time_unit;
            min_page_index = page_num;
        }
    }

    return min_page_index;
}

int main(int argc, char const *argv[])
{
	if(argc < 3){
		printf("USAGE: ./a.out INPUT_FILENAME REPLACEMENT_ALGORITHM\n");
		printf("Replacement Algorithms: RAND FIFO LRU PER FUT\n");
		exit(1);
	}

	const char * input_filename = argv[1];
    const char * replace_algo = argv[2];

	FILE *fpointer = fopen(input_filename, "r");
	if (fpointer == NULL) {
        printf("Error: Could not open file.\n");
        exit(1);
    }
	
    // Statistic Tracking
    int page_faults = 0;
    int disk_references = 0;
    int dirty_page_writes = 0;
    int operations_completed = 0;
    int page_hits = 0;

	int result = 1;
	int process_num, address;
    char read_write;
    int page_found;
    int replace_page_index;
    struct main_memory_page pages[NUM_PAGES];
    for(int i = 0; i < NUM_PAGES; i++){
        pages[i].process_number = -1;
    }
    int time_unit = 0;
    do{
        // Read a line
        
		result = fscanf(fpointer, "%d %d %c", &process_num, &address, &read_write);
        // printf("Successfully read line: %d %d %c\n", process_num, address, read_write);
        if (result != 3){
            // printf("EOF\n");
            break;
        }
        
        __uint16_t offset = (__uint16_t)address & 0b0000000111111111; // Least significant 9 bits
        __uint16_t virtual_page_number = (__uint16_t)address >> 9; // Most significant 7 bits

        page_found = get_physical_page_index(pages, process_num, virtual_page_number, offset);
        
        if(strcmp(replace_algo, "PER") == 0 && time_unit % 200 == 0){
            for(int i = 0; i < NUM_PAGES; i++){
                pages[i].reference_bit = 0;
            }
        }

        if (page_found == -1){
            // printf("Page not found!\n");
            page_faults += 1;
            // WE MUST REPLACE A PAGE

            if(strcmp(replace_algo, "RAND") == 0){
                // REPLACE ON RANDOM
                replace_page_index = get_replacement_random(pages, process_num, virtual_page_number, offset);
            }
            else if(strcmp(replace_algo, "FIFO") == 0){
                // REPLACE BY FIFO
                replace_page_index = get_replacement_fifo(pages, process_num, virtual_page_number, offset);
            }
            else if(strcmp(replace_algo, "LRU") == 0){
                // REPLACE BY LEAST RECENTLY USED
                replace_page_index = get_replacement_lru(pages, process_num, virtual_page_number, offset);
            }
            else if(strcmp(replace_algo, "PER") == 0){
                // REPLACE ON PERIODIC REFERENCE RESET
                replace_page_index = get_replacement_per(pages, process_num, virtual_page_number, offset);
            }
            else if(strcmp(replace_algo, "FUT") == 0){
                long file_start;
                file_start = ftell(fpointer);
                struct main_memory_page future_pages[FUTURE_SEEK_AMOUNT];
                int future_line_count;
                int fut_process_num, fut_address;
                char fut_read_write;
                __uint16_t fut_offset, fut_virtual_page_number;

                for(future_line_count = 0; future_line_count < FUTURE_SEEK_AMOUNT; future_line_count++){
                    result = fscanf(fpointer, "%d %d %c", &fut_process_num, &fut_address, &fut_read_write);
                    // printf("Successfully read line: %d %d %c\n", process_num, address, read_write);
                    if (result != 3){
                        // printf("EOF\n");
                        break;
                    }
                    fut_offset = (__uint16_t)fut_address & 0b0000000111111111; // Least significant 9 bits
                    fut_virtual_page_number = (__uint16_t)fut_address >> 9; // Most significant 7 bits
                    future_pages[future_line_count].process_number = fut_process_num;
                    future_pages[future_line_count].offset = fut_offset;
                    future_pages[future_line_count].virtual_page_number = fut_virtual_page_number;
                }

                fseek(fpointer, file_start, SEEK_SET); // Retrun File Pointer to original place
                replace_page_index = get_replacement_future(pages, process_num, virtual_page_number, offset, future_pages, future_line_count);
            }
            else{printf("Invalid replacement algorithm...\n"); return 1;}
            
            // printf("Replacing with page %d\n", replace_page_index);

            if (pages[replace_page_index].process_number != -1 && pages[replace_page_index].dirty_bit == 1){
                // Must write the page before overwriting
                dirty_page_writes += 1;
                disk_references += 1;
            }

            // printf("Initializing Page\n");
            // FETCH THE PAGE
            disk_references += 1;
            pages[replace_page_index].process_number = process_num;
            pages[replace_page_index].dirty_bit = 0;
            pages[replace_page_index].reference_bit = 1;
            pages[replace_page_index].virtual_page_number = virtual_page_number;
            pages[replace_page_index].offset = offset;
            pages[replace_page_index].last_used_time_unit = time_unit;

            // printf("Page Initialization Complete\n");
        }
        else{
            replace_page_index = page_found;
            page_hits += 1;
            pages[replace_page_index].last_used_time_unit = time_unit;
            // printf("Page found!\n");
        }

        if(read_write == 'W'){
            pages[replace_page_index].dirty_bit = 1;
        }
        pages[replace_page_index].reference_bit = 1;
        operations_completed += 1;
        time_unit++;
    }while(1);

    // Write back the dirty pages at the end
    for(int i = 0; i < NUM_PAGES; i++){
        if (pages[i].dirty_bit == 1 && pages[i].process_number != -1){
            disk_references += 1;
            dirty_page_writes += 1;
        }
    }
	
    
    fclose(fpointer);

    printf("Operations: %d\nPage Faults: %d\nPage Hits: %d\nDisk References: %d\nDirty Page Writes: %d\n", 
        operations_completed, page_faults, page_hits, disk_references, dirty_page_writes);
	return 0;
}
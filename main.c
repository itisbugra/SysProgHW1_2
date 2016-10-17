//
//  main.c
//  SYSHW1_2
//
//  Created by Buğra Ekuklu on 17.10.2016.
//  Copyright © 2016 The Digital Warehouse. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define kFILE_NAME "input2d.txt"
#define kRULE_FILE_NAME "rule.txt"

#ifdef BUFSIZ
#undef BUFSIZ
#define BUFSIZ 200
#endif

void generate_r(int **bin_buffer, int bin_buf_frame_width, int bin_buf_frame_height, int *rule_buffer, int **draw_buffer);

int main(int argc, const char * argv[]) {
    FILE *f_ptr = NULL;
//    char file_name[BUFSIZ] = {0};
    int bin_buffer[BUFSIZ][BUFSIZ];
    int draw_buffer[BUFSIZ][BUFSIZ];
    int rule_buffer[BUFSIZ];
    
//    printf("Enter the file name containing binary matrix: ");
//    fgets(file_name, BUFSIZ, stdin);
//    
//    if (strlen(file_name) == 0) {
//        perror("no file name specified");
//        exit(1);
//    }
//    
//    file_name[strlen(file_name) - 1] = '\0';
    
    if (!(f_ptr = fopen(kFILE_NAME, "r"))) {
        perror("file couldn't be opened");
        exit(9);
    }
    
    printf("Reading binary buffer from file...\n");
    
    int axis_cursor = 0;
    int axis = 0;
    int ordinate = 0;
    int eol_flag = 0;
    
    while (!feof(f_ptr)) {
        char ch = getc(f_ptr);
        
        if (axis == BUFSIZ || ordinate == BUFSIZ) {
            perror("file too long");
            exit(1);
        }
        
        switch (ch) {
            case '\n':
            case '\t':
                //  Set axis if not set
                if (axis == 0) {
                    axis = axis_cursor;
                }
                
                //  Initialize axis cursor
                axis_cursor = 0;
                
                //  Increment ordinate by 1
                ordinate += 1;
                
                //  Raise EOL flag
                eol_flag = 1;
                break;
                
            case EOF:
                //  Decrement ordinate by 1 if EOL flag is set
                if (eol_flag == 1) {
                    ordinate -= 1;
                }
                
                goto first_bailout;
                
            case ' ':
                continue;
                
            case '0':
            case '1':
                //  Mark the point in the matrix
                bin_buffer[ordinate][axis_cursor] = atoi(&ch);
                
                //  Forward axis cursor
                axis_cursor += 1;
                
                //  Flush EOL flag
                eol_flag = 0;
                break;
                
            default:
                perror("unexpected input sequence");
                exit(1);
        }
    }
    
first_bailout:
    //  Release the file handle
    fclose(f_ptr);
    
    if (!(f_ptr = fopen(kRULE_FILE_NAME, "r"))) {
        perror("file couldn't be opened");
        exit(9);
    }
    
    for (size_t a = 0; a < ordinate; ++a) {
        for (size_t b = 0; b < axis; ++b) {
            printf("%d", bin_buffer[a][b]);
        }
        
        printf("\n");
    }

    printf("\n");

    for (size_t i = 0; i < ordinate; ++i) {
        printf("%x\n", bin_buffer[i]);
    }
    
    printf("\n");
    
    printf("Reading rule buffer from file...\n");
    
    int index = 0;
    
    while (!feof(f_ptr)) {
        if (index > 32) {
            perror("rule buffer too long");
            exit(1);
        }
        
        char ch = getc(f_ptr);
        
        if (axis == BUFSIZ || ordinate == BUFSIZ) {
            perror("file too long");
            exit(1);
        }
        
        switch (ch) {
            case '\n':
            case '\t':
            case EOF:
                
                goto last_bailout;
            case ' ':
                continue;
                
            case '0':
            case '1':
                //  Mark the point in the matrix
                rule_buffer[index] = atoi(&ch);
                
                //  Forward index
                index += 1;
                break;
                
            default:
                perror("unexpected input sequence");
                exit(1);
        }
    }
last_bailout:
    //  Release the file handle
    fclose(f_ptr);
    
    for (size_t i = 0; i < index; ++i) {
        printf("%d", rule_buffer[i]);
    }

    printf("\n");

    printf("Binary buffer addr: %x\naxis addr: %x\nordinate addr: %x\nrule buffer addr: %x\ndraw buffer addr: %x\n", &bin_buffer, &axis, &ordinate, &rule_buffer, &draw_buffer);
    
    printf("\n");
    
    generate_r((int **)bin_buffer, axis, ordinate, rule_buffer, (int **)draw_buffer);

    for (size_t a = 0; a < ordinate; ++a) {
        for (size_t b = 0; b < axis; ++b) {
            printf("%d", draw_buffer[a][b]);
        }
        
        printf("\n");
    }
    
    return 0;
}











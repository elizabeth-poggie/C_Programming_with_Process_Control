#include "A4_sort_helpers.h"

// Function: read_all() 
// Provided to read an entire file, line by line.
// No need to change this one.
void read_all( char *filename ){
    
    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
	
    while( curr_line < MAX_NUMBER_LINES && 
           fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) )
    {
        curr_line++;
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// Function: read_all() 
// Provided to read only the lines of a file staring with first_letter.
// No need to change this one.
void read_by_letter( char *filename, char first_letter ){

    FILE *fp = fopen( filename, "r" );
    int curr_line = 0;
    text_array[curr_line][0] = '\0';
	
    while( fgets( text_array[curr_line], MAX_LINE_LENGTH, fp ) ){
        if( text_array[curr_line][0] == first_letter ){
            curr_line++;
        }

        if( curr_line == MAX_NUMBER_LINES ){
            sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
            write( 1, buf, strlen(buf) );
            break;
        }
    }
	
    text_array[curr_line][0] = '\0';
    fclose(fp);
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.
void sort_words( ){
  for (int i=1; i<MAX_NUMBER_LINES; i++){
    if(text_array[i][0] == '\0'){
      break;
    }

    int j=i;
    
    while (j>0 && strcmp(text_array[j], text_array[j-1]) < 0){
       
      //first we need to properly store old string values in tmp
      char tmp1[MAX_LINE_LENGTH];
      strcpy(tmp1, text_array[j]);
      char tmp2[MAX_LINE_LENGTH];
      strcpy(tmp2, text_array[j-1]);

      //now swap those bad bois
      strcpy(text_array[j], tmp2);
      strcpy(text_array[j-1], tmp1);
      j=j-1;

    }
  }
}

sem_t *semies[27];

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2.
int initialize( ){

  //initializing semies
  sem_unlink("semie_boi_a");
  semies[1] = sem_open("semie_boi_a", O_CREAT, 0666, 1);
  
  int letter = 96;
  for (int i=2; i<27; i++) {
    char string[50];
    sprintf(string, "semie_boi_%c", (char)(i+letter));
    sem_unlink(string);
    semies[i] = sem_open(string, O_CREAT, 0666, 0);
  }
  
  sem_unlink("semie_boi_final");
  semies[0] = sem_open("semie_boi_final", O_CREAT, 0666, 0);

  //initaliziing new file
  FILE *fp = fopen("mr_test.txt", "w");
  if (fp == NULL){
    printf("error opening files\n");
    return -1;
  }
  fclose(fp);

  return 0;
}

// YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.   
int process_by_letter( char* input_filename, char first_letter ){

  int index = (first_letter - 97 + 1);
  sem_wait(semies[index]);

  //sprintf( buf, "This process will sort the letter %c.\n",  first_letter );
  //write(1,buf,strlen(buf));
 
  //opening old test file
  FILE *fp = fopen("mr_test.txt", "a");
  read_by_letter( input_filename, first_letter );
  sort_words( );
  //fwrite(text_array, sizeof(text_array), 1, fp);
  int i = 0;
  while (text_array[i][0] == first_letter){
    fwrite(text_array[i], 1, strlen(text_array[i]), fp);
    i = (i+1);
  }
  fflush(fp);
  fclose(fp);
  fflush(fp);
  
  if ((index+1) <= 26){
    sem_post(semies[index+1]);
  } else if ((index+1) > 26){
    sem_post(semies[0]);
  }
  
    // For Q3, uncomment the following 2 lines and integrate them with your overall solution.
    // read_by_letter( input_filename, first_letter );
    // sort_words( );

    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
int finalize( ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "Sorting complete!" line
    // is printed at the very end, after all letter lines.
    
  // have a sem_wait here that hangs out until the z function happens
  
  sem_wait(semies[0]);

  //char large_boi[sizeof(text_array)];
  //fread(large_boi, 1, sizeof(text_array), fp);

  read_all("mr_test.txt");
  int boi = 0;
  while (text_array[boi][0] != '\0'){
    write(1, text_array[boi], strlen(text_array[boi]));
    boi++;
  }

  //write(1, text_array, sizeof(text_array));

  //move sorting complete to end
  sprintf( buf, "Sorting complete!\n" );
  write(1, buf, strlen(buf));


    // For Q3, come up with a way to accumulate the sorted results from each
    // letter process and print the overal sorted values to standard out.
    // You are not allowed to read from the input file, or call sort_words
    // directly from this function.
  fflush(stdout);
    return 0;
}


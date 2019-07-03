Purpose: This program convolves data
 from two input files and writes the
 convolved data to a new output file. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct TData
{ int MinIndex, MaxIndex, Points;
    float *pValue;
} Input[2], Output;

// Reads in data from input files
void data(char *file, int n) {
    int j;
    int i_temp;
    float f_temp;
    
    FILE *fp;
    
    if((fp = fopen(file, "rt")) == NULL) {
        printf("Cannot open file %d: %s\n", n, file);
        exit (1);
    }
    
    Input[n].Points = 0;
    
    // Finds Min Index
    while(!feof(fp)) {
        fscanf(fp, "%d %f", &i_temp, &f_temp);
        if (Input[n].Points == 0)
            Input[n].MinIndex = i_temp;
            Input[n].Points++;
    }
    
    // Decrease by one bc it is incremented once after eof is reached
    Input[n].Points--;
    
    // Max Index
    Input[n].MaxIndex = Input[n].MinIndex + Input[n].Points - 1;
    
    // Set file position at beginning of file
    rewind(fp);
    
    // P Value
    Input[n].pValue = (float*)malloc(Input[n].Points * sizeof(float)) - Input[n].MinIndex;
    
    for(j = Input[n].MinIndex; j < Input[n].MinIndex + Input[n].Points; j++){
        fscanf(fp, "%d %f\n", &i_temp, &Input[n].pValue[j]);
    }
    
    fclose(fp);
}

// Writes convolved data to output file
void writeData(char *file){
    int i;
    FILE *fp;
    
    if((fp = fopen(file, "wt")) == NULL){
        printf("Cannot open file\n");
        exit(1);
    }
    
    // Prints convolved data to user and output file
    for(i = Output.MinIndex; i <= Output.MaxIndex; i++){
        printf("%d\t%f\n", i, Output.pValue[i]);
        fprintf(fp, "%d\t%f\n", i, Output.pValue[i]);
    }
    printf("\n");
    
    fclose(fp);
}

int main(int argc, char *argv[]) {
    int i,j;
    
    if (argc != 4) {
        printf("You either entered too many files or not enough.\n");
        exit(1);
    }
    
    data(argv[1], 0);
    data(argv[2], 1);
    
    printf("\nConvolved Output: \n");
  
    // Min and Max Index
    Output.MinIndex = Input[0].MinIndex + Input[1].MinIndex;
    Output.MaxIndex = Input[0].MaxIndex + Input[1].MaxIndex;
    
    // Number of points
    Output.Points = Output.MaxIndex - Output.MinIndex + 1;
    
    // P Value
    Output.pValue = (float*)malloc(Output.Points * sizeof(float));
    Output.pValue -= Output.MinIndex;
    
    // Convolve
    for(i = Output.MinIndex; i <= Output.MaxIndex; i++) {
        Output.pValue[i] = 0;
        for(j = Input[0].MinIndex; j <= Input[0].MaxIndex; j++){
            if(((i-j) >= Input[1].MinIndex) && ((i-j) <= Input[1].MaxIndex))
                Output.pValue[i] += Input[0].pValue[j] * Input[1].pValue[i-j];
        }
    }
    
    writeData(argv[3]);
    
    return 0;
}

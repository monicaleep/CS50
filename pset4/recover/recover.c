#include <stdio.h>
#include <stdint.h>


int main(int argc, char *argv[]){
    if (argc != 2){
        fprintf(stderr,"Usage ./recover image\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
        if (file == NULL)
    {
        fprintf(stderr, "Could not open.\n" );
        return 2;
    }
    
    typedef uint8_t BYTE;
    int count =0;
    FILE *current = NULL;
    BYTE buffer[512];
    while (fread(&buffer,1,512,file) == 512){
        if (buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3] & 0xf0)==0xe0 ){
            if (current != NULL){
                fclose(current);
            }
            char filename[8];
            sprintf(filename,"%03i.jpg",count);
            current = fopen(filename,"w");
            fwrite(&buffer,1,512,current);
            count=count+1;
        }
        else{
            if (current != NULL){
                fwrite(&buffer, 1, 512, current);
            }
        }
    }
    if (current != NULL){
        fclose(current);
    }
    fclose(file);
    return 0;
    
}
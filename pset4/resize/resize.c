/**
 * Resizes a BMP file
 */
       
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    int factor = atoi(argv[1]);
    
    if (factor < 0 || factor >100){
        fprintf(stderr, "Usage: 0<n<100\n");
        return 1;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine padding for scanlines
    int oldHeight = abs(bi.biHeight);
    int oldWidth = bi.biWidth;
    int old_padding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (factor * oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //update header data accordingly
    bi.biWidth = bi.biWidth  * factor;
    bi.biHeight = factor * bi.biHeight;
    bi.biSizeImage = ((sizeof(RGBTRIPLE)*bi.biWidth)+new_padding)*abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < oldHeight; i++)
    {
        //repeat each row factor times to get required height
        for (int j=0; j<factor; j++){
            
            //define current cursor position for fseek rewinding
            long cursor_pos = ftell(inptr);
            
            // iterate over pixels in scanline
            for (int l=0; l<oldWidth;l++){
                
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile "factor" times
                for (int k =0; k<factor; k++){
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            //add padding
            for (int k = 0; k <new_padding; k++){
                fputc(0x00, outptr);
            }
            // skip over old padding, if any
            fseek(inptr, old_padding, SEEK_CUR);
            
            //rewind to start of scanline;
            if (j<factor-1){
                fseek(inptr, cursor_pos, SEEK_SET);
            }
        }
    }
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

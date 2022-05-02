#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMPINPUT "naamloos.bmp"
#define BMPOUTPUT "output.bmp"

int main(int argc, char const *argv[])
{
    FILE * inputBMP = fopen(BMPINPUT, "rb");
    unsigned char header[54] = {0};
    signed int hoogte = 0;
    signed int breedte = 0;
    unsigned char * pixels = NULL;
    int totaalAantalPixels = 0;

    if(inputBMP == NULL)
    {
        printf("%s\n", "ERROR: can't open the file");
        return -1;
    }
    int grid=0;
    printf("met welke grid wilt u de afbeelding smoothen:");
    scanf("%d", &grid);
    fread(header, 1, 54, inputBMP);

    breedte = header[21] << 24 | header[20] << 16 | header[19] << 8 | header[18]; 
    printf("De breedte van mijn afbeelding is = %d\n", breedte);
    hoogte = header[25] << 24 | header[24] << 16 | header[23] << 8 | header[22]; 
    printf("De hoogte van mijn afbeelding is = %d\n", hoogte);

    totaalAantalPixels = breedte * hoogte;
    pixels = (unsigned char *) malloc(totaalAantalPixels*3);
    if(pixels == NULL)
    {
        printf("ERROR: memory allocation Failed\n");
        return -2;
    }

    fread(pixels, 1, totaalAantalPixels*3, inputBMP);
    printf("INFO: Heap memory allocated = %d (bytes)\n", totaalAantalPixels*3);

    fclose(inputBMP);
    printf("INFO: File %s CLOSED\n", BMPINPUT);

    //----------------------------------------
    
    //print pixels in HEX value.
/*
    int i = 0;
    printf("%d\n", totaalAantalPixels);
    while(i<totaalAantalPixels*3){
        printf("%X|%X|%X\n",pixels[i],pixels[i+1], pixels[i+2]);
        i=i+3;
    }
*/
    int x,y,gem;
    int gridx, gridy;    
    int gemR, gemB, gemG;

    for(gridx = 0; gridx <breedte; gridx++)
    {
        for(gridy = 0; gridy < hoogte; gridy++)
        {
            gemB = gemG = gemR = 0;
            gem = 0;

            for(x = gridx; x < breedte && x < gridx + grid/*blurSize*/; x++)
            {


               for(y = gridy; y < hoogte && y < gridy + grid/*blurSize*/; y++)
                {
                    gemB += pixels[x*3 + y*breedte*3 + 0];
                    gemG += pixels[x*3 + y*breedte*3 + 1];
                    gemR += pixels[x*3 + y*breedte*3 + 2];
                    gem++;
                }
            }
            //gemiddelde berekenen per pixelKleur
            gemB = gemB / gem;
            gemG = gemG / gem;
            gemR = gemR / gem;
            //de gemiddeldes toewijzen aan de pixel
            pixels[gridx*3 + gridy*breedte*3 + 0] = gemB;
            pixels[gridx*3 + gridy*breedte*3 + 1] = gemG;
            pixels[gridx*3 + gridy*breedte*3 + 2] = gemR;
        }
    }
    printf("\n");
    printf("result\n");
    printf("\n");

/*  
    //printen van de output in hex
    i = 0;
    printf("%d\n", totaalAantalPixels);
    while(i<totaalAantalPixels*3){
        printf("%X|%X|%X\n",pixels[i],pixels[i+1], pixels[i+2]);
        i=i+3;
    }
*/
    //output file aanmaken of openen 
    FILE * OUTPUT = fopen(BMPOUTPUT, "wb");
    //de bmp header toewijzen aan de output file
    fwrite(header,sizeof(char),sizeof(header),OUTPUT);
    //fseek zorgt dat de 'cursor' verplaatst wordt naar achter de header.
    fseek(OUTPUT,sizeof(char)*54,SEEK_SET);
    //schrijft de nieuwe pixels
    fwrite(pixels,sizeof(char),totaalAantalPixels*3,OUTPUT);


    //----------------------------------------

    fclose(OUTPUT);
    free(pixels);
    printf("INFO: Heap memory Freed = %d (bytes)\n", totaalAantalPixels*3);
    return 0;
}
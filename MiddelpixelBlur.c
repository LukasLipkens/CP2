#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMPINPUT "naamloos.bmp"
#define BMPOUTPUT "output.bmp"

void blur(unsigned char * pixels, signed int breedte, signed int hoogte);
void mono(unsigned char * pixels, signed int breedte, signed int hoogte);
void grey(unsigned char * pixels, signed int breedte, signed int hoogte);
void gaas(unsigned char * pixels, signed int breedte, signed int hoogte);


int main(int argc, char const *argv[])
{

  printf(" _    _ _____ _     _____ ________  __ _____\n");
printf("| |  | |  ___| |   /  __ \\  _  |  \\/  |  ___|\n");
printf("| |  | | |__ | |   | /  \\/ | | | .  . |  |__\n");
printf("| |/\\| |  __|| |   | |   | | | | |\\/| |   __|\n");
printf("\\  /\\  / |___| |___| \\__/\\ \\_/ / |  | |  |___\n");
printf(" \\/  \\/\\____/\\_____/\\____/\\___/\\_|  |_/\\____/\n\n\n");


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


    int keuze=0;
    printf("BLUR ------------- [1]\n");
    printf("MONOCHROME ------- [2]\n");
    printf("GREYSCALE--------- [3]\n");
    printf("OLD TV FIlTER -----[4]\n");
    printf("=> ");
    scanf("%d",&keuze);

    if(keuze==1)
    {
        blur(pixels, breedte, hoogte);
    }
    if(keuze==2)
    {
        mono(pixels, breedte, hoogte);
    }
    if(keuze==3)
    {
        grey(pixels,breedte, hoogte);
    }
    if(keuze==4)
    {
      gaas(pixels,breedte, hoogte);
    }
    printf("\n");
    printf("result\n");
    printf("\n");


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


void blur(unsigned char * pixels, signed int breedte, signed int hoogte)
{
    int x,y,deler;
    int gridx, gridy;
    int gemR, gemB, gemG;
    int grid=0;

    printf("met welke grid wilt u de afbeelding smoothen:");
    scanf("%d", &grid);

    for(gridx = 0; gridx <breedte; gridx++) //x coordinaat
    {
        for(gridy = 0; gridy < hoogte; gridy++) // y coordinaat
        {
            gemB = gemG = gemR = 0;
            deler = 0;

            for(x = gridx -grid/2; x < breedte && x < gridx + grid/*blurSize*/; x++)
            {
                //printf("i get to here\n");
                if(x>0){
                for(y = gridy -grid/2; y < hoogte && y < gridy + grid/*blurSize*/; y++)
                {
                    //printf("i get to here p2\n");
                    if(y>0){
                    gemB += pixels[x*3 + y*breedte*3 + 0];

                    gemG += pixels[x*3 + y*breedte*3 + 1];

                    gemR += pixels[x*3 + y*breedte*3 + 2];

                    deler++; //na elke pixel deler optellen voor gemiddelde brekenen
                    }
                    else{}


                }
                }
                else{}
            }
            //gemiddelde berekenen per pixelKleur
            gemB = gemB / deler;
            gemG = gemG / deler;
            gemR = gemR / deler;
            //de gemiddeldes toewijzen aan de pixel
            pixels[gridx*3 + gridy*breedte*3 + 0] = gemB;
            pixels[gridx*3 + gridy*breedte*3 + 1] = gemG;
            pixels[gridx*3 + gridy*breedte*3 + 2] = gemR;
        }
    }
}

void mono(unsigned char * pixels, signed int breedte, signed int hoogte)
{
    int x,y,deler;
    int gridx, gridy;
    int gemR, gemB, gemG, gemA;

        for(gridx = 0; gridx <breedte; gridx++)
    {
        for(gridy = 0; gridy < hoogte; gridy++)
        {
            gemB = gemG = gemR = 0;
            deler = 0;

            //voor monochrome te maken moet de blursize om 1 staan denk.
            for(x = gridx; x < breedte && x < gridx + 1; x++)
            {


               for(y = gridy; y < hoogte && y < gridy + 1; y++)
                {
                    gemB += pixels[x*3 + y*breedte*3 + 0];
                    gemG += pixels[x*3 + y*breedte*3 + 1];
                    gemR += pixels[x*3 + y*breedte*3 + 2];
                    deler++;
                }
            }



            gemA = (gemB + gemG + gemR) / 3;
            if (gemA > 128) {
              pixels[gridx*3 + gridy*breedte*3 + 0] = 255;
              pixels[gridx*3 + gridy*breedte*3 + 1] = 255;
              pixels[gridx*3 + gridy*breedte*3 + 2] = 255;
            }
            else{
            pixels[gridx*3 + gridy*breedte*3 + 0] = 0;
            pixels[gridx*3 + gridy*breedte*3 + 1] = 0;
            pixels[gridx*3 + gridy*breedte*3 + 2] = 0;
            }
        }
    }
}

void grey(unsigned char * pixels, signed int breedte, signed int hoogte)
{
    int x,y,deler;
    int gridx, gridy;
    int gemR, gemB, gemG, gemA;

        for(gridx = 0; gridx <breedte; gridx++)
    {
        for(gridy = 0; gridy < hoogte; gridy++)
        {
            gemB = gemG = gemR = 0;
            deler = 0;

            //voor monochrome te maken moet de blursize om 1 staan denk.
            for(x = gridx; x < breedte && x < gridx + 1/*blurSize*/; x++)
            {


               for(y = gridy; y < hoogte && y < gridy + 1/*blurSize*/; y++)
                {
                    gemB += pixels[x*3 + y*breedte*3 + 0];
                    gemG += pixels[x*3 + y*breedte*3 + 1];
                    gemR += pixels[x*3 + y*breedte*3 + 2];
                    deler++;
                }
            }

            gemA = (gemB + gemG + gemR) / 3;
            pixels[gridx*3 + gridy*breedte*3 + 0] = gemA;
            pixels[gridx*3 + gridy*breedte*3 + 1] = gemA;
            pixels[gridx*3 + gridy*breedte*3 + 2] = gemA;

        }
    }
}
void gaas(unsigned char * pixels, signed int breedte, signed int hoogte)
{
    int x,y,deler;
    int gridx, gridy;
    int gemR, gemB, gemG, gemA;

        for(gridx = 0; gridx <breedte; gridx++)
    {
        for(gridy = 0; gridy < hoogte; gridy++)
        {
            gemB = gemG = gemR = 0;
            deler = 0;


            for(x = gridx; x < breedte && x < gridx + 1; x++)
            {


               for(y = gridy; y < hoogte && y < gridy + 1; y++)
                {
                    gemB += pixels[x*3 + y*breedte*3 + 0];
                    gemG += pixels[x*3 + y*breedte*3 + 1];
                    gemR += pixels[x*3 + y*breedte*3 + 2];
                    deler++;
                }
            }

            gemA = ((gemB *3) + gemG /gemG + gemR *gemR);
            pixels[gridx*3 + gridy*breedte*3 + 0] = gemA;
            pixels[gridx*3 + gridy*breedte*3 + 1] = gemA;
            pixels[gridx*3 + gridy*breedte*3 + 2] = gemA;

        }
    }
}

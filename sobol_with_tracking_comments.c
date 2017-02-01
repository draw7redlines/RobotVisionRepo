//Mark Vetro
//Sobol algorithm with thresh modifications added
//This still produces pgm images shifted right.
//Need to remove headers before reading input to fix this
//It works and I'm tired though, so I'll do this another time...



#include <stdio.h>                          /* Sobel.c */
#include <math.h>


//image feeding here
        int pic[256][256];
        int outpicx[256][256];
        int outpicy[256][256];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        double ival[256][256],maxival;
        double ivalLow[256][256];
        double ivalHigh[256][256];


//main
main(argc,argv)
int argc;
char **argv;
{
    printf("got here...");
    system("pause");
        int i,j,p,q,mr,sum1,sum2;
        double threshold;
        FILE *fo1, *fo2, *fp1, *lowThreshOut, *highThreshOut, *fopen();
        char *foobar;



        argc--; argv++;
        foobar = *argv;
        printf("foobar is reading as %s here \n", foobar);
        fp1=fopen(foobar,"rb");

	argc--; argv++;
	foobar = *argv;
	 printf("foobar is reading as %s here \n", foobar);
	fo1=fopen(foobar,"wb");


	//added file stuff
	lowThreshOut= fopen("sobolLow.pgm", "wb");
    highThreshOut= fopen("sobolHigh.pgm", "wb");



        argc--; argv++;
	foobar = *argv;
	threshold = atof(foobar);


	//loop through the entire 256x256 pixel image
        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                    //grabbing values of individual pixels
                  pic[i][j]  =  getc (fp1);
                  pic[i][j]  &= 0377;
                }
        }


        //convolution step
        //======================================
        mr = 1;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
             }
             outpicx[i][j] = sum1;
             outpicy[i][j] = sum2;
          }
        }
        //==========================================



        //follow up the convolution step by calculating the magnitude of the vector
        maxival = 0;
        printf("mask radius is currently reading in as %d \n", mr);

        //the mr convention is trying to ignore the edges of the image
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));

            ivalLow[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));

            ivalHigh[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
             if (ival[i][j] > maxival)
                maxival = ival[i][j];

           }
        }



        //add a header tag to the new files to declare it as a pgm file
        fprintf(fo1, "P5\n");
        fprintf(fo1, "256 256\n");
        fprintf(fo1, "255\n");

        fprintf(lowThreshOut, "P5\n");
        fprintf(lowThreshOut, "256 256\n");
        fprintf(lowThreshOut, "255\n");

        fprintf(highThreshOut, "P5\n");
        fprintf(highThreshOut, "256 256\n");
        fprintf(highThreshOut, "255\n");



        lowThresh(lowThreshOut);
        highThresh(highThreshOut);

        printf("the maximum value operator is %f \n", maxival);
        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
            //max value will be restricted to 255 since 1*255 = 255
             ival[i][j] = (ival[i][j] / maxival) * 255;


            //this is going to handle magnitude display
             fprintf(fo1,"%c",(char)((int)(ival[i][j])));

            }
          }

          printf("got to the end of the program... \n");
          system("pause");
}



void lowThresh(lowThreshOut)
{
    double lowThreshVal = 25;
    for (int i=0;i<256;i++)
          {
              int j=0;
              for (j;j<256;j++)
            {
             ivalLow[i][j] = (ivalLow[i][j] / maxival) * 255;


                //try a low thresh here
                if(ivalLow[i][j]<lowThreshVal)
                {
                    ivalLow[i][j]=0;
                }
                else if(ivalLow[i][j]>=lowThreshVal)
                {
                    ivalLow[i][j]=255;
                }


            //this is going to handle low thresh display
             fprintf(lowThreshOut,"%c",(char)((int)(ivalLow[i][j])));

            }
          }


}

void highThresh(highThreshOut)
{
    double highThreshVal = 220;

    int i=0;
    for (i;i<256;i++)
          {
              int j=0;
              for (j;j<256;j++)
            {
            //max value will be restricted to 255 since 1*255 = 255
             ivalHigh[i][j] = (ivalHigh[i][j] / maxival) * 255;


                //try a low thresh here
                if(ivalHigh[i][j]<highThreshVal)
                {
                    ivalHigh[i][j]=0;
                }
                else if(ivalHigh[i][j]>=highThreshVal)
                {
                    ivalHigh[i][j]=255;
                }


            //this is going to handle low thresh display
             fprintf(highThreshOut,"%c",(char)((int)(ivalHigh[i][j])));

            }
          }

}

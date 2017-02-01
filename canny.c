//Mark Vetro
//Robot Vision Canny Algorithm




#include <stdio.h>                  /*  Canny.c */
#include <math.h>
#include <stdlib.h>

//constants of variable image size and mask definition
#define  PICSIZE 256
#define  MAXMASK 100




double generateThresh( threshPercent);
void trueCanny(highThreshVal, lowThreshVal);


         int    pic[PICSIZE][PICSIZE];
         double outpic1[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         double ival[PICSIZE][PICSIZE];
         double mag[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double convx[PICSIZE][PICSIZE];
         double convy[PICSIZE][PICSIZE];
         double maskx[MAXMASK][MAXMASK];
         double masky[MAXMASK][MAXMASK];
         double realCannyOut[PICSIZE][PICSIZE];
         int histTable[255];
         double peaks[PICSIZE][PICSIZE];
         double finalOutput[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr,centx,centy;
        double  maskvalx, maskvaly, sum1, sum2, sig, inputPercentage, maxival,minival,maxval,ZEROTOL;
        FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
        char    *foobar;


        //file input for image
        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        printf("opening file %s \n", foobar);

        //file output for outfile name, likely need to add pgm tags later down the line here...
        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb");

        printf("writing to %s \n", foobar);


        //second file write, for peak display
        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb");

        //third file for true canny output
        argc--; argv++;
        foobar = *argv;
        fo3=fopen(foobar,"wb");

        //assigning sig var to a float value of string conversion
        argc--; argv++;
        foobar = *argv;
        sig = atof(foobar);

        printf("the sig value is reading as %f \n", sig);


        //assigning input percentage variable
        printf("\n\n What percentage should the canny threshold be operated off of? \n");
        printf("Enter in integer format please! Ex) 25 == 25 percent 50 == 50 percent and so on \n");
        printf("For accuracy, 10 percent seems to be the best threshold percentage compromise \n");
        scanf("%lf", &inputPercentage);
        inputPercentage = (inputPercentage/100);

        printf("The canny threshold percentage is reading as... %f \n", inputPercentage);


        //masked radius being computed using sig assignment, with centx and centy independently being assigned, hinging on the defined
        //value of maxmask constant
        //operate off the mask value here to make it flexible
        mr = (int)(sig * 3);
        centx = (MAXMASK / 2);
        centy = (MAXMASK / 2);



        //loop through the image and get all of the pixel values
        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                }
        }


        for (p=-mr;p<=mr;p++)
        {

             for (q=-mr;q<=mr;q++)
           {

               //keep everything on the exp function no matter what
               //this handles the e^whatever, and is needed no matter
               //which algorithm possibility is being tested
              maskvalx = (q*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig))))));

              maskvaly = (p*(exp(-1*(((p*p)+(q*q))/(2*(sig*sig))))));


              (maskx[p+centy][q+centx]) = maskvalx;
              (masky[p+centy][q+centx]) = maskvaly;

           }
        }



        //this is convolution and needs to be doubled up
        //to yield 2 outputs which can then be funneled through
        //the magnitude formula
        for (i=mr;i<=255-mr;i++)
        { for (j=mr;j<=255-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;

             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+centy][q+centx];
                   sum2 += pic[i+p][j+q] * masky[p+centy][q+centx];
                }
             }

             outpic1[i][j] = sum1;
             outpic2[i][j] = sum2;
             convx[i][j] = sum1;
             convy[i][j] = sum2;

          }
        }


        maxival=0;


//===============================================================================================================
        //Get the magnitude here, this is super important for first output, and being able
        //to run realCanny
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             ival[i][j]=sqrt((double)((outpic1[i][j]*outpic1[i][j]) +
                                      (outpic2[i][j]*outpic2[i][j])));

             mag[i][j]=sqrt((double)((outpic1[i][j]*outpic1[i][j]) +
                                      (outpic2[i][j]*outpic2[i][j])));

             if (ival[i][j] > maxival)
                maxival = ival[i][j];

           }
        }

//===============================================================================================================



        //add a header tag to the new files to declare it as a pgm file
        fprintf(fo1, "P5\n");
        fprintf(fo1, "256 256\n");
        fprintf(fo1, "255\n");

        fprintf(fo2, "P5\n");
        fprintf(fo2, "256 256\n");
        fprintf(fo2, "255\n");

        fprintf(fo3, "P5\n");
        fprintf(fo3, "256 256\n");
        fprintf(fo3, "255\n");



        //do the printing stuff for the magnitude output here
        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             ival[i][j] = (ival[i][j] / maxival) * 255;

             fprintf(fo1,"%c",(char)((int)(ival[i][j])));

            }
          }



        //run realCanny and get the "noisy" output
          realCanny(mr);

          for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {

             fprintf(fo2,"%c",(char)((int)(realCannyOut[i][j])));

            }
          }



        //zero out final output table and run trueCanny and get the filtered output

        for(int i=0; i<256; i++)
        {
            for(int j=0; j<256; j++)
            {
                finalOutput[i][j]=0;
            }
        }

          double highThreshVal = generateThresh(inputPercentage);
          double lowThreshVal = (highThreshVal * .35);


          trueCanny(highThreshVal, lowThreshVal);


          //run the print out of the final output

          for(i=0; i<256; i++)
          {
              for(j=0; j<256; j++)
              {
                  fprintf(fo3, "%c", (char)((int)(finalOutput[i][j])));
              }
          }



}

//check directions to decide which conic section to follow for
//brightness assignment
//Check using slope which will be calculated off the values of
//xconvolution and yconvolution tables.
void realCanny(mr)
{
    double slope;

    //loop through all the image's pixels
    for(int i=mr; i<256-mr; i++)
    {
        for(int j=mr; j<256-mr; j++)
        {
            //make sure not to divide by zero anywhere
            if((convx[i][j])==0.0)
            {
                convx[i][j] = .00001;
            }

            slope = (convy[i][j]/convx[i][j]);

            if((slope<= .4142) && (slope> -.4142))
            {
                if((mag[i][j]>mag[i][j-1]) && (mag[i][j]>mag[i][j+1]))
                {
                    realCannyOut[i][j]=255;
                    peaks[i][j]=999;
                }
            }

            else if((slope <= 2.4142) && (slope > .4142))
            {
               if((mag[i][j]>mag[i-1][j-1]) && (mag[i][j]>mag[i+1][j+1]))
                {
                    realCannyOut[i][j]=255;
                    peaks[i][j]= 999;
                }
            }

            else if((slope <= -.4142) && (slope > -2.4142))
            {
                if((mag[i][j]>mag[i+1][j-1]) && (mag[i][j]>mag[i-1][j+1]))
                {
                    realCannyOut[i][j]=255;
                    peaks[i][j]=999;
                }
            }

            else
            {
                if((mag[i][j]>mag[i-1][j]) && (mag[i][j]>mag[i+1][j]))
                {
                    realCannyOut[i][j]=255;
                    peaks[i][j]=999;
                }
            }

        }
    }


}

void makeHistogram()
{
    int flag;
    for(int i=0; i<255; i++)
    {
        for(int j=0; j<255; j++)
        {
            flag = ival[i][j];
            histTable[flag] ++;

        }
    }

}


double generateThresh( double threshPercent)
{
    double thresh=0;
    double areaOfTops=0;

    makeHistogram();
    double cutOff = threshPercent*255*255;

    for(int i=255; i>0; i--)
    {
        areaOfTops += histTable[i];

        if(areaOfTops>cutOff)

        {
            thresh= i;
            i=-1;
        }
    }

    return thresh;
}



//true canny will give me an actually good output with no noise
void trueCanny(double highThreshVal, double lowThreshVal)
{
    for(int i=0; i<255; i++)
    {

        for(int j=0; j<255; j++)
        {
            if(peaks[i][j]==999)
            {
                if(ival[i][j]>highThreshVal)
                {
                    peaks[i][j]= -1;
                    finalOutput[i][j] = 255;

                }

                else if(ival[i][j]<lowThreshVal)
                {
                    peaks[i][j] = -1;
                    finalOutput[i][j]=0;
                }
            }

        }
    }

    int moreToDo = 999;

    while(moreToDo==999)
    {
        moreToDo = -1;

        for(int i=0; i<255; i++)
        {

            for(int j=0; j<255; j++)
            {

                if(peaks[i][j] == 999)
                {
                    for(int x=-1; x<2; x++)
                    {
                        for(int y=-1; y<2; y++)
                        {
                            if(finalOutput[i+x][j+y]==255)
                            {
                                peaks[i][j] = -1;
                                finalOutput [i][j];
                                moreToDo = 999;
                            }

                        }
                    }
                }

            }
        }
    }

}

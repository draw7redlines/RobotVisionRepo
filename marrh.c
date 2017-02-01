//Mark Vetro
//Marr Hildreth algorithm
// not really what I'm playing with, but still interesting to poke through it I guess???


#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>

//constants of variable image size and mask definition
#define  PICSIZE 256
#define  MAXMASK 100



         int    pic[PICSIZE][PICSIZE];
         double outpic1[PICSIZE][PICSIZE];
         double outpic2[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double mask[MAXMASK][MAXMASK];
         double conv[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
        int     i,j,p,q,s,t,mr,centx,centy;
        double  maskval,sum,sig,maxival,minival,maxval,ZEROTOL;
        FILE    *fo1, *fo2,*fp1, *fopen();
        char    *foobar;


        //file input for image
        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

        //file output for outfile name, likely need to add pgm tags later down the line here...
        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb");

        //second file write, not sure what for yet
        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb");

        //assigning sig var to a float value of another input read in??
        argc--; argv++;
        foobar = *argv;
        sig = atof(foobar);

        //same as above
        argc--; argv++;
        foobar = *argv;
        ZEROTOL = atof(foobar);

        //masked radius being computed using sig assignment, with centx and centy independently being assigned, hinging on the defined
        //value of maxmask constant
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
        {  for (q=-mr;q<=mr;q++)
           {
              maskval = ((2-(((p*p)+(q*q))/(sig*sig)))*
                      (exp(-1*(((p*p)+(q*q))/(2*(sig*sig))))));
              (mask[p+centy][q+centx]) = maskval;
           }
        }

        for (i=mr;i<=255-mr;i++)
        { for (j=mr;j<=255-mr;j++)
          {
             sum = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum += pic[i+p][j+q] * mask[p+centy][q+centx];
                }
             }
             outpic1[i][j] = sum;
             conv[i][j] = sum;
          }
        }

        maxval  = 0;
        maxival = 0;
        minival = 255;

        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             if (outpic1[i][j] > maxival)
                maxival = outpic1[i][j];
             if (outpic1[i][j] < minival)
                minival = outpic1[i][j];
           }
        }
        if (fabs(maxival) > fabs(minival))
           maxval = fabs(maxival);
        else
           maxval = fabs(minival);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
          {
             outpic1[i][j] = ((((outpic1[i][j]) / maxval) + 1) * 127);
             fprintf(fo1,"%c",(char)((int)(outpic1[i][j])));
          }
        }

        for (i=mr;i<=255-mr;i++)
        {  for (j=mr;j<=255-mr;j++)
           {
                         outpic2[i][j] = 0;
             if (conv[i][j] > ZEROTOL)
             {
               for (p=-1;p<=1;p++)
               {
                 for (q=-1;q<=1;q++)
                 {
                   if (conv[i+p][j+q] < -ZEROTOL)
                   {
                     outpic2[i][j] = 255;
                   }
                 }
               }
             }
             else if ((fabs)(conv[i][j]) < ZEROTOL)
             {
                     if (((conv[i+1][j] > ZEROTOL) &&
                          (conv[i-1][j] < -ZEROTOL))   ||
                         ((conv[i+1][j] < -ZEROTOL) &&
                          (conv[i-1][j] > ZEROTOL)))
                     {
                       outpic2[i][j] = 255;
                     }
                     else if (((conv[i][j+1] > ZEROTOL) &&
                               (conv[i][j-1] < -ZEROTOL))   ||
                              ((conv[i][j+1] < -ZEROTOL) &&
                               (conv[i][j-1] > ZEROTOL)))
                     {
                       outpic2[i][j] = 255;
                     }
                     else if (((conv[i+1][j+1] > ZEROTOL) &&
                               (conv[i-1][j-1] < -ZEROTOL))   ||
                              ((conv[i+1][j+1] < -ZEROTOL) &&
                               (conv[i-1][j-1] > ZEROTOL)))
                     {
                       outpic2[i][j] = 255;
                     }
                     else if (((conv[i+1][j-1] > ZEROTOL) &&
                               (conv[i-1][j+1] < -ZEROTOL))   ||
                              ((conv[i+1][j-1] < -ZEROTOL) &&
                               (conv[i-1][j+1] > ZEROTOL)))
                     {
                       outpic2[i][j] = 255;
                     }
             }
           }
        }

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
          {  if (outpic2[i][j] == 255) outpic2[i][j]=0;
             else outpic2[i][j]=255;
             fprintf(fo2,"%c",(char)((int)(outpic2[i][j])));
          }
        }
}


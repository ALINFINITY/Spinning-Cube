#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

float cubeWith = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distanceFromCam = 110;

float incrementSpeed = 0.6;

float K1 = 40;

float ooz;
int xp, yp;
int idx;

float x,y,z;

float calculateX(int i, int j, int k)
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - 
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - 
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k)
{
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float CubeX, float CubeY, float CubeZ, int ch){
    x = calculateX(CubeX, CubeY, CubeZ);
    y = calculateY(CubeX, CubeY, CubeZ);
    z = calculateZ(CubeX, CubeY, CubeZ) + distanceFromCam;
    
    ooz = 1/z;
    
    xp = (int)(width/2 + 2 * width + K1 * ooz * x * 2);
    yp = (int)(height/2 + K1 * ooz * y);

    idx = xp + yp * width;

    if(idx>= 0 && idx < width * height){
        if(ooz> zBuffer[idx]){
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    system("cls");
    while (1)
    {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        for (float CubeX = -cubeWith; CubeX < cubeWith; CubeX += incrementSpeed)
        {
            for (float CubeY = -cubeWith; CubeY < cubeWith; CubeY += incrementSpeed)
            {
                calculateForSurface(CubeX, CubeY, -cubeWith, '.');
                calculateForSurface(cubeWith, CubeY, CubeX, '$');
                calculateForSurface(-cubeWith, CubeY, -CubeX, '~');
                calculateForSurface(-CubeX, CubeY, cubeWith, '#');
                calculateForSurface(CubeX, -cubeWith, -CubeY, ';');
                calculateForSurface(CubeX, cubeWith, CubeY, '+');
            }
        }
        printf("\x1b[H");
        for(int k = 0; k < width * height; k++){
            putchar(k % width ? buffer[k]:10);
        }

        A += 0.05;
        B += 0.05;
        C += 0.1;
        usleep(6000);
    }
    return 0;
}

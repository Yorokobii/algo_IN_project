#ifndef BRESENHAM_H
#define BRESENHAM_H

#include "Image.h"

void Z2ToFirstOct(int xA, int xB, int yA, int yB, int *xA1o, int *xB1o, int *yA1o, int *yB1o){
	int xA1q, xB1q, yA1q, yB1q;
	if(xB > xA){xA1q=xA; xB1q=xB;}
	else{xA1q=-xA; xB1q=-xB;}
	
	if(yB > yA){yA1q=yA; yB1q=yB;}
	else{yA1q=-yA; yB1q=-yB;}

	if(xB1q-xA1q > yB1q - yA1q){
		*xA1o = xA1q; *xB1o = xB1q;
		*yA1o = yA1q; *yB1o = yB1q;
	}
	else{
		*xA1o = yA1q; *xB1o = yB1q;
		*yA1o = xA1q; *yB1o = xB1q;
	}
}

void FirstOctToZ2(int xA, int xB, int yA, int yB, int x1o, int y1o, int *x, int *y){
	int x1q, y1q;
	if(fabs(xB-xA) > fabs(yB-yA)){
		x1q = x1o;
		y1q = y1o;
	}
	else{
		x1q = y1o;
		y1q = x1o;
	}

	if(xB > xA)
		*x = x1q;
	else
		*x = -x1q;

	if(yB > yA)
		*y = y1q;
	else
		*y = -y1q;
}

void I_bresenham(Image *img, int xA, int xB, int yA, int yB){
	int xA1o, yA1o, xB1o, yB1o;
	Z2ToFirstOct(xA, xB, yA, yB, &xA1o, &xB1o, &yA1o, &yB1o);

	int dx = xB1o-xA1o, dy = yB1o - yA1o;
	float incrd1 = 2*dy, incrd2=2*(dy-dx);
	int x = xA1o, y = yA1o, d = 2*dy-dx;

	int xZ2,yZ2;
	while(x<xB1o){
		FirstOctToZ2(xA, xB, yA, yB, x, y, &xZ2, &yZ2);
		I_plot(img, xZ2, yZ2);
		x++;
		if(d<0) d+= incrd1;
		else{ y++; d+=incrd2; }
	}
	FirstOctToZ2(xA, xB, yA, yB, x, y, &xZ2, &yZ2);
	I_plot(img, xZ2, yZ2);
}

#endif //BRESENHAM_H
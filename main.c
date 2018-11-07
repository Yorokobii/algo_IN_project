
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdbool.h>

#include "Image.h"
#include "List.h"
#include "Bresenham.h"

Image *img;
List* poly;
enum state{
	opened,
	closed,
	filled
};
enum state poly_state = opened;
Color white;
Color black;
Color red;
Color green;
Color blue;

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
//taken from internet
char get_line_intersection(Point a, Point b, Point c, Point d, Point* inter)
{
    Point ab, cd;
    ab.x = b.x - a.x;     ab.y = b.y - a.y;
    cd.x = d.x - c.x;     cd.y = d.y - c.y;

    float s, t;
	float den = (-cd.x * ab.y + ab.x * cd.y);
	if(den != 0){
		s = (-ab.y * (a.x - c.x) + ab.x * (a.y - c.y)) / den;
		t = ( cd.x * (a.y - c.y) - cd.y * (a.x - c.x)) / den;

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			// Collision detected
			inter = malloc(sizeof(Point));
			if (inter != NULL)
				inter->x = a.x + (t * ab.y);
			if (inter != NULL)
				inter->x = a.y + (t * ab.y);
			return 1;
		}
	}

    return 0; // No collision
}

void sortIntersections(Point* inters, int n){
	int i, j, min_idx;

    // One by one move boundary of unsorted subarray 
    for (i = 0; i < n-1; i++) 
    { 
        // Find the minimum element in unsorted array 
        min_idx = i; 
        for (j = i+1; j < n; j++) 
          if (inters[j].x < inters[min_idx].x) 
            min_idx = j; 
  
        // Swap the found minimum element with the first element
		Point tmp = inters[min_idx];
		inters[min_idx] = inters[i];
		inters[i] = tmp;
    }
}

void scan_line(Image* _img,List* _poly, Color _c){
	Node* node = _poly->first;
	Point intersections[_poly->size];

	//for all lines horizontally
	for(int i=0; i<img->_height; ++i){
		node = _poly->first;
		int intersections_count = 0;

		while(node && node->next){
			//segment from node to node->next
			
			//intersection between horizontal line and current segment
			Point* p = NULL;
			Point left;
			left.x = 0;
			left.y = i;
			Point right;
			right.x = _img->_width;
			right.y = i;
			// if(get_line_intersection(left, right, node->point, node->next->point, p)){
			// 	//intersection
			// 	intersections[intersections_count++] = *p;
			// }
			if(node->next) node = node->next;
		}
		if(node){
			//segment from node to poly->first
			
			//intersection between horizontal line and current segment
			Point* p = NULL;
			Point left;
			left.x = 0;
			left.y = i;
			Point right;
			right.x = _img->_width;
			right.y = i;
			// if(get_line_intersection(left, right, node->point, poly->first->point, p)){
			// 	//intersection
			// 	intersections[intersections_count++] = *p;
			// }
		}

		// //sort the intersections along the x
		// sortIntersections(intersections, intersections_count);

		// //draw the lines from the intersections
		// for(int i=0; i<intersections_count-2; i+=2){
		// 	I_bresenham(img, intersections[i].x, intersections[i].y, intersections[i+1].x, intersections[i+1].y);
		// }
	}




}

void I_plotPoly(Image* _img, List* _poly, Color _c){
	I_changeColor(img, _c);
	
	Node* node = poly->first;
	while(node && node->next){
		I_bresenham(img, node->point.x, node->next->point.x, node->point.y, node->next->point.y);
		if(node->next) node = node->next;
	}
	//if closed or fill close the polygon
	if(((poly_state == closed) || (poly_state == filled)) && node)
		I_bresenham(img, node->point.x, poly->first->point.x, node->point.y, poly->first->point.y);

	//fill the polygon
	if(poly_state == filled){
		scan_line(_img, _poly, red);
	}

}

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

	I_fill(img, black);

	I_plotPoly(img, poly, white);	

	I_draw(img);

    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN)){
		// I_focusPoint(img,x,img->_height-y);
		Point p;
		p.x = x;
		p.y = img->_height - y;
		ListInsert(poly, p);
	}

	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch(key)
	{
	case 27 : exit(1); break;
	case 'z' : I_zoom(img,2.0); break;
	case 'Z' : I_zoom(img,0.5); break;
	case 'i' : I_zoomInit(img); break;
	case 'c' :
		if(poly_state>=2)
			poly_state = opened;
		else{
			if(poly_state == opened)
				poly_state = closed;
			else
				poly_state = opened;
		}

		break;
	case 'f' : poly_state = filled; break;
	default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 10;

	switch(key)
	{
	case GLUT_KEY_UP    : I_move(img,0,d); break;
	case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
	case GLUT_KEY_LEFT  : I_move(img,d,0); break;
	case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
	default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if((argc!=3)&&(argc!=2))
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\nou",argv[0]);
		fprintf(stderr,"\t: %s <ppmfilename> \n\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if(argc==2)
		{
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur,hauteur);
		}
		int windowPosX = 100, windowPosY = 100;
		white = C_new(255,255,255);
		black = C_new(0,0,0);
		red = C_new(255,0,0);
		green = C_new(0,255,0);
		blue = C_new(0,0,255);

		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,0,hauteur,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		poly = ListNew();
		glutMainLoop();
		ListDestroy(poly);
		free(poly);

		return 0;
	}
}

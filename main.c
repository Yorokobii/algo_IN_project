
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

int largeur, hauteur;
Image *img;
List* poly;
enum state{
	opened,
	closed,
	filled
};
enum state poly_state = opened;
enum mode{
	insert,
	vertex,
	edge
};
enum mode current_mode = insert;
Color white;
Color black;
Color red;
Color green;
Color blue;
Color insert_color;
Color vertex_color;
Color edge_color;

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
			if (inter != NULL)
				inter->x = a.x + (t * ab.x);
			if (inter != NULL)
				inter->y = a.y + (t * ab.y);
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

		bool point_inter = false;

		while(node && node->next){
			//segment from node to node->next
			
			//intersection between horizontal line and current segment
			Point* p = malloc(sizeof(Point));
			Point left;
			left.x = 0;
			left.y = i;
			Point right;
			right.x = _img->_width;
			right.y = i;
			
			if(get_line_intersection(left, right, node->point, node->next->point, p)){
				//intersection
				//check if the intersection is a point of the polygon
				if(p->x == node->point.x && p->y == node->point.y &&
				(node->point.y > node->next->point.y)){
					//do nothing
				}
				else if(p->x == node->next->point.x && p->y == node->next->point.y &&
				(node->next->point.y > node->point.y)){
					// do nothing
				}
				else
					intersections[intersections_count++] = *p;
			}
			free(p);
			if(node->next) node = node->next;
		}
		if(node && !point_inter){
			//segment from node to poly->first

			//intersection between horizontal line and current segment
			Point* p = malloc(sizeof(Point));
			Point left;
			left.x = 0;
			left.y = i;
			Point right;
			right.x = _img->_width;
			right.y = i;
			if(get_line_intersection(left, right, node->point, poly->first->point, p)){
				//intersection
				if(p->x == node->point.x && p->y == node->point.y &&
				(node->point.y > poly->first->point.y)){
					//do nothing
				}
				else if(p->x == poly->first->point.x && p->y == poly->first->point.y &&
				(poly->first->point.y > node->point.y)){
					//do nothing
				}
				else
					intersections[intersections_count++] = *p;
			}
			free(p);
		}

		int j;
		//sort the intersections along the x
		sortIntersections(intersections, intersections_count);

		int nb_lines = 0;
		//draw the lines from the intersections
		for(j=0; j<intersections_count-1; j+=2){
			nb_lines++;
			I_bresenham(img, intersections[j].x, intersections[j+1].x, intersections[j].y, intersections[j+1].y);
		}

		if(intersections_count > 2){
			for(j=0; j<intersections_count; j++){
				nb_lines++;
			}
		}
	}

}

void I_plotPoints(Image* _img, List* _poly, Color _cp, Color _cc){
	Node* node = _poly->first;
	
	I_changeColor(img, _cp);
	while(node){

		for(int x=node->point.x-2; x<=node->point.x+2; x++){
			for(int y=node->point.y-2; y<=node->point.y+2; y++){
				//first point red
				if(node == _poly->first)
					I_plotColor(_img, x, y, red);
				else if(ListIsLast(_poly, node))
					I_plotColor(_img, x, y, blue);
				else I_plot(_img, x, y);
			}
		}

		if(node == _poly->current && current_mode == vertex){
			I_changeColor(img, _cc);

			I_bresenham(_img, node->point.x-3, node->point.x+3, node->point.y-3, node->point.y-3);
			I_bresenham(_img, node->point.x-3, node->point.x+3, node->point.y+3, node->point.y+3);

			I_bresenham(_img, node->point.x-3, node->point.x-3, node->point.y-3, node->point.y+3);
			I_bresenham(_img, node->point.x+3, node->point.x+3, node->point.y-3, node->point.y+3);

			I_changeColor(img, _cp);
		}
		node = node->next;
	}
}

void I_plotPoly(Image* _img, List* _poly, Color _c){
	I_plotPoints(_img, _poly, green, red);
	I_changeColor(img, _c);
	
	Node* node = poly->first;
	while(node && node->next){
		if(current_mode == edge && node == _poly->current)
			I_changeColor(img, red); // change color for selected edge
		else
			I_changeColor(img, _c); // change color back
		I_bresenham(img, node->point.x, node->next->point.x, node->point.y, node->next->point.y);
		if(node->next) node = node->next;
	}
	//if closed or fill close the polygon
	if(((poly_state == closed) || (poly_state == filled)) && node){
		if(current_mode == edge && node == _poly->current)
			I_changeColor(img, red); // change color for selected edge
		else
			I_changeColor(img, _c); // change color back
		I_bresenham(img, node->point.x, poly->first->point.x, node->point.y, poly->first->point.y);
	}


	I_changeColor(img, _c);
	//fill the polygon
	if(poly_state == filled){
		scan_line(_img, _poly, red);
	}
}

void I_plotMode(Image* _img){
	switch(current_mode){
		case insert:
			I_changeColor(_img, insert_color);
			break;
		case vertex:
			I_changeColor(_img, vertex_color);
			break;
		case edge:
			I_changeColor(_img, edge_color);
			break;
	}
	for(int i=0; i<30; ++i)
		I_bresenham(_img, 0, 30, i, i);
		
}

// returns the closest vertex index of poly from _x and _y
//acts also as closestEdge in my implementation ????? MEH
int closestVertex(List* _poly, int _x, int _y){
	Node* node = _poly->first;
	int index = 0;
	float dist = sqrt((node->point.x - _x)*(node->point.x - _x) + (node->point.y - _y)*(node->point.y - _y));
	node = node->next;
	float while_dist;
	int i = 0;
	while(node){
		i++;
		while_dist = sqrt((node->point.x - _x)*(node->point.x - _x) + (node->point.y - _y)*(node->point.y - _y));
		if(while_dist < dist){
			dist = while_dist;
			index = i;
		}
		node = node->next;
	}
	return index+1;
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

	I_fill(img, white);

	I_plotMode(img);

	I_plotPoly(img, poly, black);


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
		if(current_mode==insert){
			// I_focusPoint(img,x,img->_height-y);
			Point p;
			p.x = x;
			p.y = img->_height - y;
			ListInsertAt(poly, p, poly->size);
		}
		else{ // vertex or edge select closestVertex
			ListSelectCurrent(poly, closestVertex(poly, x, img->_height - y));
		}
	}

	if((button==GLUT_MIDDLE_BUTTON)&&(state==GLUT_DOWN)&&current_mode==edge&&poly->size > 2){
		Point tmp;
		if(!ListIsLast(poly, poly->current) || poly_state != opened){
			tmp.x = poly->current->next ?
					((poly->current->point.x + poly->current->next->point.x) / 2) :
					((poly->current->point.x + poly->first->point.x) / 2);
			tmp.y = poly->current->next ?
					((poly->current->point.y + poly->current->next->point.y) / 2) :
					((poly->current->point.y + poly->first->point.y) / 2);
			ListNext(poly); //to insert after the current and not before
			ListInsert(poly, tmp);
		}
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
		Point tmp;
	switch(key)
	{
	case 27 : exit(1); break;
	case 'g' : // because my middle mouse button on my computer doesn't work
		if(!ListIsLast(poly, poly->current) || poly_state != opened){
			tmp.x = poly->current->next ?
					((poly->current->point.x + poly->current->next->point.x) / 2) :
					((poly->current->point.x + poly->first->point.x) / 2);
			tmp.y = poly->current->next ?
					((poly->current->point.y + poly->current->next->point.y) / 2) :
					((poly->current->point.y + poly->first->point.y) / 2);
			ListNext(poly); //to insert after the current and not before
			ListInsert(poly, tmp);
		}
		break;
	case 'i' : current_mode = insert; break;
	case 'v' : current_mode = vertex; break;
	case 'e' : current_mode = edge; break;
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
	//suppr
	case 127 : if(current_mode == vertex) ListDelete(poly); break;
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

	switch(key)
	{
	case GLUT_KEY_UP    :
		if(current_mode == vertex) 
			if(poly->current->point.y < hauteur-1)
				poly->current->point.y++;
		break;
	case GLUT_KEY_DOWN  :
		if(current_mode == vertex) 
			if(poly->current->point.y > 1)
				poly->current->point.y--;
		break;
	case GLUT_KEY_LEFT  :
		if(current_mode == vertex) 
			if(poly->current->point.x > 1)
				poly->current->point.x--;
		break;
	case GLUT_KEY_RIGHT :
		if(current_mode == vertex) 
			if(poly->current->point.x < largeur-1)
				poly->current->point.x++;
		break;
	//page_up
	case 104 : if(current_mode == vertex || current_mode == edge) ListNext(poly); break;
	//page_down
	case 105 : if(current_mode == vertex || current_mode == edge) ListPrev(poly); break;
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
		insert_color = C_new(255,255,0);
		vertex_color = C_new(0,255,255);
		edge_color = C_new(255,0,255);

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


		//pb deletecurrent with last || first node

		// Point p1;
		// p1.x = 1;
		// p1.y = 1;
		// Point p2;
		// p2.x = 2;
		// p2.y = 2;
		// Point p3;
		// p3.x = 3;
		// p3.y = 3;

		// ListInsertAt(poly, p1, 0);
		// ListInsert(poly, p2);
		// ListInsert(poly, p3);

		// ListDisplay(poly);

		// ListDelete(poly);

		// ListDisplay(poly);

		ListDestroy(poly);
		free(poly);

		return 0;
	}
}

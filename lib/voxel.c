/* Program to create a wireframe cube and torus
 */
//#define VOXEL
#define OCT_TREE
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "voxel.h"

#define ESCAPE 27

GLdouble eyex = 1.0;
GLdouble eyey = 3.0;
GLdouble eyez = 3.0;


char octTreeMade = 'N';

struct octTree* objTree;

static GLfloat lightpos[] =
{10.f, 10.f, 10.f, 1.f};
static GLfloat lightcol[] = {1.0,1.0,1,0};  

/********************************** Global objects needed by the CREATED voxel library. ******************************************/
//#################################  TEST MATHEMATICAL FUNCTIONS #################################################################//

struct boundingBox start;

unsigned int ellipseFunc(struct point3D pt)
{
	unsigned int a = 2.5, b = 1.5, c = 2.0;
	if ( (pt.x)*(pt.x)/(a*a) + (pt.z)*(pt.z)/(c*c) + (pt.y)*(pt.y)/(b*b) <= 1 )
		return 1;
	else
		return 0;
}

unsigned int cylinderFunc(struct point3D pt)
{
	if ( sqrt((pt.x)*(pt.x) + (pt.z)*(pt.z)) <= 2.0  && pt.y <= 2.0 )
		return 1;
	else
		return 0;
}

unsigned int sphereFunc(struct point3D pt)
{
	if ( sqrt( (pt.x)*(pt.x) + (pt.y)*(pt.y) + (pt.z)*(pt.z) ) <= 2.5 )
		return 1;
	else
		return 0;
}
/**********************************************************************************************************************************/



void keyboardPress (unsigned char key, int x, int y)
{
   switch (key)
   {
      case ESCAPE :
         exit (0);
   }
}

void InitGL (int Width,int Height)
{
   glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
   glClearDepth (1.0f);
   glEnable (GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glShadeModel (GL_FLAT);
   glMatrixMode(GL_MODELVIEW);
}


/*******************************************************************************************/

struct octTree* voxelSpacePartition(unsigned int (* equation)(struct point3D point),struct boundingBox bBox)
{
	struct octTree* beginRoot = init_octTree();

	/* memcpy((void*) &beginRoot->root, (void*) &bBox, sizeof(struct boundingBox)); */
	beginRoot->domain = bBox; /* Copying of structures */

	VSP_glOctTree(equation,beginRoot);

	//############################# HERE ON THE RENDERING PROCESS WILL START ###############################

	/* We'll have to traverse the tree in order to render the figure. */
	return beginRoot;
}

/***********************************************************************************************/


/* glMaterialfv: Specify material parameters for the lighting model. */
void  DrawGLScene(void)
{
   /* Clear stencile each time */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   
   glLoadIdentity ();
   
   /* Set eye and viewing direction */
   /* Line of sight : (5,3,-2) to (2,2,0) */
   gluLookAt (eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
   glTranslatef(0.0f, -2.0f, -4.0f);

   /* void glutSolidCube(GLdouble size); is used. */
   
   if ( octTreeMade == 'N' )
   	{ objTree = voxelSpacePartition(&cylinderFunc,start);
	  octTreeMade = 'Y';
	}
   octTreeTraversal(objTree);
		
   glFlush();						// Flush the OpenGl Buffers to the window.
   glutSwapBuffers();

}

void ResizeGLScene (int Width, int Height)
{
   

glViewport (0, 0, (GLsizei) Width, (GLsizei) Height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   /* glFrustum - multiply the current matrix by a perspective matrix */
   /* void glFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far ) */
   /* Specifies the positions of the Clipping Planes. */

   glFrustum (-0.5, 0.5, -0.5, 0.5, 0.5, 20.0);
   
   glMatrixMode (GL_MODELVIEW);
}

int main (int argc, char **argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (750, 750);               // TODO : Reference
   
   glutCreateWindow (argv[0]);                  // TODO : Reference
   
   /* This function is called only once the OpenGL window is created. */
   InitGL (750, 750);
   
   /******************************* INITIALIZATION *****************************************************/
   /* Keeping these outside a function i.e in global space results in errors. These initializations therfore need to be
    * within the definiton of a function. */  
   start.center.x = 0.0;

   // start.center.y = 0.0;  /* For cylinder and sphere */
   start.center.y = 0.0;      /*  For cone */
   start.center.z = 0.0;
   start.edge_len = 5.0;

   static GLfloat colorf[4] = {0.5, 0.8, 0.2, 1.0};
   static GLfloat colorb[4] = {0.2, 0.5, 0.5, 1.0};
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,colorf);
   glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE,colorb);
   /****************************************************************************************************/

   /* Set the display function */
   glutDisplayFunc (&DrawGLScene);

   glutIdleFunc(&DrawGLScene);

   /* Set the reshape function */
   glutReshapeFunc (&ResizeGLScene);

   /* Set the keyboard function */
   glutKeyboardFunc (&keyboardPress);
   

   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glLightfv (GL_LIGHT0, GL_POSITION, lightpos);
   glLightfv (GL_LIGHT0, GL_AMBIENT, lightcol);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

   glMatrixMode(GL_MODELVIEW);
   
   glutMainLoop ();
   return 0;	
}

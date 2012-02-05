#include "common.h"

#define MIN_EDGE_LENGTH 0.5
enum state {EMPTY,PARTIAL,FULL};


/* Here the unit vector is specified with repect to the origin as the reference point */
struct point3D
{
	GLfloat x;	// x component
	GLfloat y;	// y component
	GLfloat z;	// z component

};

struct boundingBox
{
	struct point3D center;
	GLfloat edge_len;
} start;			/* start is the CUBICAL DOMAIN provided by the user */

struct octTree
{
	/* This cube itself */ 
	struct boundingBox domain;

	/* State of this cube */
	enum  state S;
	
	/* Cuboid's eight octants.*/	
	struct octTree* f_BottomLeft;
	struct octTree* f_TopLeft;
	struct octTree* f_TopRight;
	struct octTree* f_BottomRight;
	struct octTree* b_BottomLeft;
	struct octTree* b_TopLeft;
	struct octTree* b_TopRight;
	struct octTree* b_BottomRight;
};


struct point3D* translate(struct point3D source,GLfloat x, GLfloat y, GLfloat z)
{
	struct point3D* dest = (struct point3D*)malloc(sizeof(struct point3D));
	dest->x = source.x + x; 
	dest->y = source.y + y; 
	dest->z = source.z + z; 

	return dest;
}

struct octTree* giveOctant(struct octTree* root, char* tag)
{
	GLfloat e = (root->domain).edge_len;
	struct octTree* OT = (struct octTree *)malloc(sizeof(struct octTree));
	(OT->domain).edge_len = e/2;
	OT->S = PARTIAL;
	
	OT->f_BottomLeft = NULL;	OT->f_TopLeft = NULL; 
	OT->f_TopRight = NULL;		OT->f_BottomRight = NULL; 
	OT->b_BottomLeft = NULL;	OT->b_TopLeft = NULL;
	OT->b_TopRight = NULL;		OT->b_BottomRight = NULL;
	
	if ( strcmp(tag,"FBL") == 0 )
		  (OT->domain).center = *(translate((root->domain).center,-e/4,-e/4,e/4)) ; 
	else 
		if ( strcmp(tag,"FTL") == 0 )
			(OT->domain).center = *(translate((root->domain).center,-e/4,e/4,e/4)) ;
		else 	
			if ( strcmp(tag,"FTR") == 0 )
				(OT->domain).center = *(translate((root->domain).center,e/4,e/4,e/4)) ;
			else 	
				if ( strcmp(tag,"FBR") == 0 )
					(OT->domain).center = *(translate((root->domain).center,e/4,-e/4,e/4)) ;
				else 	
					if ( strcmp(tag,"BBL") == 0 )
						(OT->domain).center = *(translate((root->domain).center,-e/4,-e/4,-e/4)) ;
					else 	
						if ( strcmp(tag,"BTL") == 0 )
							(OT->domain).center = *(translate((root->domain).center,-e/4,e/4,-e/4)) ;
						else 	
							if ( strcmp(tag,"BTR") == 0 )
								(OT->domain).center = *(translate((root->domain).center,e/4,e/4,-e/4)) ;
							else 	
								if ( strcmp(tag,"BBR") == 0 )
									(OT->domain).center = *(translate((root->domain).center,e/4,-e/4,-e/4)) ;
	
	return OT;

}


struct octTree* init_octTree()
{
	struct octTree* OT = (struct octTree*)malloc(sizeof(struct octTree));
	OT->S = PARTIAL;
	(OT->domain).center.x = (OT->domain).center.y = (OT->domain).center.z = 0;
	(OT->domain).edge_len = 0;
	OT->f_BottomLeft = NULL;	OT->f_TopLeft = NULL; 
        OT->f_TopRight = NULL;		OT->f_BottomRight = NULL;
        OT->b_TopRight = NULL;		OT->b_BottomRight = NULL;
	
	return OT;
}

void VSP_glOctTree(unsigned int (*equation)(struct point3D point), struct octTree* begin)
{
/* Divide the cuboid recursively into 8 blocks */
	if ( (begin->domain).edge_len <= MIN_EDGE_LENGTH )
		{	
			// No need of touching the pointers here , as they will anyway not be looked at during the OctTree traversal. 
			if ( equation((begin->domain).center) )
				{ begin->S = FULL; 
				  #ifdef VOXEL
				  GLfloat x = (begin->domain).center.x;	GLfloat y = (begin->domain).center.y; GLfloat z = (begin->domain).center.z;	
				  glTranslatef(x,y,z);
				  glutWireCube((GLdouble)MIN_EDGE_LENGTH);
				  glTranslatef(-x,-y,-z);
				  #endif	
				  return ;}
			else
				{ begin->S = EMPTY; return ;}
		}
	else	// We need to divide further 
		{
			
			
			/* Either the cube is fully occupied OR empty OR partially occupied (In which case we need to keep the child nodes) */

			/* Start division into octants */

			/*######################### BEGIN CONSTRUCTION OF THE EIGHT CHILD NODES OF THE CURRENT NODE. ##############################*/
			begin->f_BottomLeft = giveOctant(begin,"FBL");
			VSP_glOctTree(equation,begin->f_BottomLeft);
			
			begin->f_TopLeft = giveOctant(begin,"FTL");	
			VSP_glOctTree(equation,begin->f_TopLeft); 	
			
			begin->f_TopRight = giveOctant(begin,"FTR");					
			VSP_glOctTree(equation,begin->f_TopRight); 	
			
			begin->f_BottomRight = giveOctant(begin,"FBR");		
			VSP_glOctTree(equation,begin->f_BottomRight);
			
			begin->b_BottomLeft = giveOctant(begin,"BBL");		
			VSP_glOctTree(equation,begin->b_BottomLeft);	

			begin->b_TopLeft = giveOctant(begin,"BTL");	
			VSP_glOctTree(equation,begin->b_TopLeft);

			begin->b_TopRight = giveOctant(begin,"BTR");		
			VSP_glOctTree(equation,begin->b_TopRight); 	

			begin->b_BottomRight = giveOctant(begin,"BBR");		
			VSP_glOctTree(equation,begin->b_BottomRight);
			/*##########################################################################################################################*/

			if ( 		(begin->f_BottomLeft)->S == FULL 
				&& 	(begin->f_TopLeft)->S == FULL 
				&& 	(begin->f_TopRight)->S == FULL
				&& 	(begin->f_BottomRight)->S == FULL
				&& 	(begin->b_BottomLeft)->S == FULL
				&& 	(begin->b_TopLeft)->S == FULL
				&& 	(begin->b_TopRight)->S == FULL
				&& 	(begin->b_BottomRight)->S == FULL
			   )
				begin->S = FULL;

			else
				if ( 		(begin->f_BottomLeft)->S == EMPTY
                                	&& 	(begin->f_TopLeft)->S == EMPTY
			               	&& 	(begin->f_TopRight)->S == EMPTY
					&& 	(begin->f_BottomRight)->S == EMPTY 
	                               	&& 	(begin->b_BottomLeft)->S == EMPTY
                                	&& 	(begin->b_TopLeft)->S == EMPTY
                                	&& 	(begin->b_TopRight)->S == EMPTY
                                	&& 	(begin->b_BottomRight)->S == EMPTY
				   )
				begin->S = EMPTY;
		}
	return;
}


void octTreeTraversal(struct octTree* begin)
{
	if ( begin->S == PARTIAL )
		// We need to go down further in order to render the object.
		{
			octTreeTraversal(begin->f_BottomLeft);
			octTreeTraversal(begin->f_TopLeft);
			octTreeTraversal(begin->f_TopRight);
			octTreeTraversal(begin->f_BottomRight);
			octTreeTraversal(begin->b_BottomLeft);
			octTreeTraversal(begin->b_TopLeft);
			octTreeTraversal(begin->b_TopRight);
			octTreeTraversal(begin->b_BottomRight);
		}
	else if ( begin->S == FULL )
		{
			// We need to render this part of the object.
			#ifdef OCT_TREE
			GLfloat x = (begin->domain).center.x;  GLfloat y = (begin->domain).center.y;  GLfloat z = (begin->domain).center.z;
			GLfloat e = (begin->domain).edge_len;
			glTranslatef(x,y,z);
			glutWireCube(e);
			glTranslatef(-x,-y,-z);
			#endif
		}
	// Else the component is EMPTY. So we leave this case.
	return;
}


/*
 * quike_draw.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */

#include "quike_header.h"




void qkDrawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
	glPushMatrix();
	glTranslatef(x, y, z);

	glBegin(GL_POLYGON);

	glVertex3f(r, r, r);
	glVertex3f(-r, r, r);
	glVertex3f(-r, -r, r);
	glVertex3f(r, -r, r);

	glVertex3f(r, r, r);
	glVertex3f(r, r, -r);

	glVertex3f(-r, r, -r);
	glVertex3f(-r, r, r);
	glVertex3f(-r, r, -r);

	glVertex3f(-r, -r, -r);
	glVertex3f(-r, -r, r);
	glVertex3f(-r, -r, -r);

	glVertex3f(r, -r, -r);
	glVertex3f(r, -r, r);
	glVertex3f(r, -r, -r);

	glVertex3f(r, r, -r);
	glVertex3f(r, r, r);

	glEnd();
	glPopMatrix();
}


void qkDrawWall(const unsigned x, const unsigned y)
{
	static const GLfloat colors[3 * 3] =
	{
			0.7,0.7,0.7,
			0.3,0.3,0.3,
			0.2,0.2,0.2
	};

	if ((x+y) % 3 != 0){
		glColor3fv(colors);
		qkDrawCube((GLfloat) x, (GLfloat) y, 2, 0.5);
	}

	glColor3fv(colors + 3);
	qkDrawCube((GLfloat) x, (GLfloat) y, 1, 0.5);

	glColor3fv(colors + 6);
	qkDrawCube((GLfloat) x, (GLfloat) y, 0, 0.5);
}


void qkDrawFloor(const unsigned x, const unsigned y)

{
	glPushMatrix();

	glTranslatef((GLfloat) x, (GLfloat) y,  -0.5);

	static const GLfloat colors1[] = {
			0.7,0.7,0.7,
			0.3,0.3,0.3
	};

	static const GLfloat colors2[] = {
			0.5,0.5,0.5,
			0.2,0.2,0.2
	};



	glBegin(GL_POLYGON);

	glColor3fv(colors1 + 3*((x + y) % 2 == 0));
	glVertex3f(-0.5, -0.5, 0);
	glColor3fv(colors2 + 3*((x + y) % 2 == 0));
	glVertex3f(-0.5, 0.5, 0);
	glColor3fv(colors1 + 3*((x + y) % 2 == 0));
	glVertex3f(0.5, 0.5, 0);
	glColor3fv(colors2 + 3*((x + y) % 2 == 0));
	glVertex3f(0.5, -0.5, 0);

	glEnd();

	glPopMatrix();



}

void qkDrawRespawn(const unsigned x, const unsigned y)
{
	glPushMatrix();

		glTranslatef((GLfloat) x, (GLfloat) y,  -0.5);

		static const GLfloat colors1[] = {
				0.9,0.0,0.0,
				0.7,0.0,0.0
		};

		static const GLfloat colors2[] = {
				0.0,0.9,0.0,
				0.0,0.7,0.0
		};



		glBegin(GL_POLYGON);

		glColor3fv(colors1 + 3*((x + y) % 2 == 0));
		glVertex3f(-0.5, -0.5, 0);
		glColor3fv(colors2 + 3*((x + y) % 2 == 0));
		glVertex3f(-0.5, 0.5, 0);
		glColor3fv(colors1 + 3*((x + y) % 2 == 0));
		glVertex3f(0.5, 0.5, 0);
		glColor3fv(colors2 + 3*((x + y) % 2 == 0));
		glVertex3f(0.5, -0.5, 0);

		glEnd();

		glPopMatrix();

}


void qkDrawCeiling(const unsigned x, const unsigned y)
{
	glPushMatrix();

	glTranslatef((GLfloat) x, (GLfloat) y,  2);


	glBegin(GL_POLYGON);
	glColor3f(0.7,0.7,0.7);
	glVertex3f(0.5, -0.5, 0);
	glColor3f(0.6,0.6,0.6);
	glVertex3f(0.5, 0.5, 0);
	glColor3f(0.7,0.7,0.7);
	glVertex3f(-0.5, 0.5, 0);
	glColor3f(0.6,0.6,0.6);
	glVertex3f(-0.5, -0.5, 0);
	glEnd();

	glPopMatrix();
}

/*
 * quike_main.c
 *
 *  Created on: 9 déc. 2018
 *      Author: ajuvenn
 */


#include "quike_header.h"



#define QK_FRAME_PERIOD 40

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Quike");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
	glClearColor(0.0f, 0.7, 0.9, 1.0f);
	glEnable(GL_DEPTH_TEST);


	glutDisplayFunc(qkSceneRenderHandler);
	glutPassiveMotionFunc(qkPassiveMotionEventHandler);
	glutMotionFunc(qkMotionEventHandler);
	glutKeyboardFunc(qkKeyboardEventHandler);
	glutReshapeFunc(qkWindowReshapeHandler);


	QKPlayer * p = qkPlayerNew(10,10,0);
	qkPlayerSetGlobalPlayer(p);

	qkLoadNewMap(20,20);

	for (unsigned x = 0 ; x < 20 ; x++){
		for (unsigned y = 0 ; y < 20 ; y++){
			if (x == 0 || x == 20 - 1 || y == 0 || y == 20-1){
				qkGlobalMap.cells[x + 20*y] = QK_WALL_CELL;
			} else {
				qkGlobalMap.cells[x + 20*y] = QK_FLOOR_CELL;
			}
		}
	}
	qkGlobalMap.cells[10 + 10*20] = QK_RESPAWN_CELL;

	qkInitMouseCentering();
	qkPeriodicSceneRender(QK_FRAME_PERIOD);

	glutMainLoop();

	return 0;
}

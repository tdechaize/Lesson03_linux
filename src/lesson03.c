/*
=====================================================================
OpenGL Lesson 03 :  Creating Another OpenGL Window with glut on Linux
=====================================================================

  Authors Name: Jeff Molofee ( NeHe )

  This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)

  If you've found this code useful, please let me know.

  Visit me at www.demonews.com/hosted/nehe
 (email Richard Campbell at ulmont@bellsouth.net)

  Disclaimer:

  This program may crash your system or run poorly depending on your
  hardware.  The program and code contained in this archive was scanned
  for virii and has passed all test before it was put online.  If you
  use this code in project of your own, send a shout out to the author!

=====================================================================
                NeHe Productions 1999-2004
=====================================================================
*/

 /*******************************************************************
 *  Project: $(project)
 *  Function : Main program
 ***************************************************************
 *  $Author: Jeff Molofee 2000 ( NeHe )
 *  $Name:  $
 ***************************************************************
 *
 *  Copyright NeHe Production
 *
 *******************************************************************/

/**         Comments manageable by Doxygen
*
*  Modified smoothly by Thierry DECHAIZE
*
*  Paradigm : obtain one source (only one !) compatible for multiple free C Compilers
*    and provide for all users an development environment on Linux (64 bits compatible),
*    the great Code::Blocks manager (version 20.03), and don't use glaux.lib or glaux.dll.
*
*	a) gcc 11.3.0 (32 and 64 bits) version officielle : commande pour l'installation sur Linux Mint
*       -> sudo apt-get install freeglut3 libglew-dev gcc g++ mesa-common-dev build-essential libglew2.2 libglm-dev binutils libc6 libc6-dev ; option -m32 to 32 bits ; no option to 64 bits
*	b) CLANG version 14.0.6 (32 et 64 bits), adossé aux environnements gcc : commande pour l'installation sur Linux Mint
*       -> sudo apt-get install llvm clang ; options pour la compilation et l'édition de liens -> --target=i686-pc-linux-gnu (32 bits) --target=x86_64-pc-linux-gnu (64 bits)
*	c) Mingw 32 ou 64 bits version gcc version 10-win32 20220113 : commande pour l'installation sur Linux Mint
*        -> sudo apt-get install mingw64    (ATTENTION, il s'agit d'une cross génération : l'exécutable créé ne fonctionne que sur Windows !!!)
*
*  Date : 2022/12/28
*
* \file            lesson03.c
* \author          Jeff Molofee ( NeHe ) originely, adapted by Richard Campbell, and after by Thierry Dechaize
* \version         2.0.1.0
* \date            28 décembre 2022
* \brief           Ouvre une simple fenêtre Wayland on Linux et dessine deux formes simples : un triangle multicolore et un carré bleu avec OpenGL + glut.
* \details         Ce programme gére plusieurs événements : le clic sur le bouton "Fermé" de la fenêtre, la sortie par la touche ESC ou par les touches "q" ou "Q" [Quit]
* \details                                                  l'appui sur les touches "f" ou "F" qui active ou non le "Full Screen" en mode flip-flop.
*
*/

#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
#include <unistd.h>     // Header File for sleeping.

/* ASCII code for the escape key. */
#define ESCAPE 27

/* The number of our GLUT window */
int window;

/* Indicator of Full Screen */
int nFullScreen=0;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */

/**	            This function initialize the basics characteristics of the scene with instructions OpenGL (background, depth, type of depth, reset of the projection matrix, ...)
*
* \brief      Fonction InitGL : fonction gerant les caractéristiques de base de la scéne lorsque avec des instructions OpenGL (arrière plan, profondeur, type de profondeur, ...)
* \details    En retour les deux paramètres des nouvelles valeurs de largeur et de hauteur de la fenêtre redimensionnée.
* \param	  Width			    la largeur de la fenêtre lors de l'initialisation					*
* \param	  Height			la hauteur de la fenêtre lors de l'initialisation					*
* \return     void              aucun retour.
*
*/

void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */

/**	            This function manage the new dimension of the scene when resize of windows with instructions OpenGL
*
* \brief      Fonction ReSizeGLScene : fonction gerant les nouvelles dimensions de la scéne lorsque l'utilisateur agit sur un redimensionnement de la fenêtre
* \details    En retour les deux paramètres des nouvelles valeurs de largeur et de hauteur de la fenêtre redimensionnée.
* \param	  Width			    la  nouvelle largeur de la fenêtre redimensionnée					*
* \param	  Heigth			la  nouvelle hauteur de la fenêtre redimensionnée					*
* \return     void              aucun retour.
*
*/

void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */

/**	            This function generate the scene with injstructions OpenGL
*
* \brief      Fonction DrawGLScene : fonction generant l'affichage attendu avec des instructions OpenGL
* \details    Aucun paramètre dans ce cas de figure car tout est géré directement à l'intérieur de cette fonction d'affichage.
* \return     void              aucun retour.
*
*/

void DrawGLScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
  glLoadIdentity();				// Reset The View

  glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0

  // draw a triangle (in smooth coloring mode)
  glBegin(GL_POLYGON);				// start drawing a polygon
  glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
  glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
  glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
  glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
  glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
  glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
  glEnd();					// we're done with the polygon (smooth color interpolation)

  glTranslatef(3.0f,0.0f,0.0f);		        // Move Right 3 Units

  // draw a square (quadrilateral)
  glColor3f(0.5f,0.5f,1.0f);			// set color to a blue shade.
  glBegin(GL_QUADS);				// start drawing a polygon (4 sided)
  glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
  glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
  glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
  glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
  glEnd();					// done with the polygon

  // we need to swap the buffer to display our drawing.
  glutSwapBuffers();
}

/* The function called whenever a key is pressed. */

/**	            This function inform caller with key pressed.
*
* \brief      Fonction keyPressed : fonction permettant de connaître quelle touche a été tapée au clavier
* \details    Le premier paarmètre en retour de cette fonction correspond à la touche précédemment appuyée.
* \param	  key			    la touche appuyée 					*
* \param	  x             	une position x au niveau du clavier *
* \param	  y             	une position y au niveau du clavier *
* \return     void              aucun retour.
*
*/

void keyPressed(unsigned char key, int x, int y)
{
    /* sleep to avoid thrashing this procedure */
    usleep(100);

    switch (key){

	//Mode plein écran : il suffit de taper au clavier sur la touche F majuscule ou f minuscule en mode flip-flop
		case 'f' :
		case 'F' :
			if (nFullScreen==0)
			{
				glutFullScreen();
				nFullScreen=1;
				break;
			}
			if (nFullScreen==1)
			{
		        glutReshapeWindow(640, 480);
				glutPositionWindow (0, 0);
				nFullScreen=0;
				break;
			}
	//Quitter
		case 'q' :
		case 'Q' :
		case 27  : // Touche ESC : il s'agit de sortir proprement de ce programme
            glutDestroyWindow(window);
			exit (0);
			break;

		default :
//			printf(" Touche: %c\n Souris a: %d %d \n",key,x,y);
			break;

	}

}

/* Main function : GLUT runs as a console application starting with program call main()  */

/**         Comments manageable by Doxygen
*
* \brief      Programme Main obligatoire pour les programmes sous Linux (OpenGL en mode console).
* \details    Programme principal de lancement de l'application qui appelle ensuite toutes les fonctions utiles OpenGL et surtout glut.
* \param      argc         le nombre de paramètres de la ligne de commande.
* \param      argv         un pointeur sur le tableau des différents paramètres de la ligne de commande.
* \return     int          un entier permettant de connaître la statut du lancement du programme.
*
*/

int main(int argc, char **argv)
{
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */

/**	            This Code initialize the context of windows on Wayland with glut.
*
* \brief      L'appel de la fonction glutInit : fonction Glut d'initialisation
* \details    En entrée de cette fonction, les paramètres de la ligne de commande.
* \param	  argc			    le nombre de paramètres mis à disposition			*
* \param	  argv              Hle tableau des différents paramètres mis à disposition *
* \return     int               un integer.
*
*/

  glutInit(&argc, argv);

  /* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffer */

/**	            This Code initialize le mode d'affichage défini avec une fonction glut.
*
* \brief      L'appel de la fonction glutInitDisplayMode(: fonction Glut initialisant le mode d'affichage.
* \details    En entrée de cette fonction, des paramètres séparés par des "ou logique".
* \param	  const         une succession deparamètres séparés par des "ou logique".
* \return     void          aucun retour de fonction.
*
*/

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* get a 640 x 480 window */

/**	            This Code initialize the dimensions (width & height) of the window into screen.
*
* \brief      L'appel de la fonction glutInitWindowSize : fonction Glut initialisant la position de la fenêtre dans l'écran.
* \details    En entrée de cette fonction, les deux paramètres X et Y correspondant à la taille de la fenêtre dans l'écran (deux dimensions)
* \param	  X			    la largeur de la fenêtre en x
* \param	  Y			    la hauteur de la fenêtre en y
* \return     void          aucun retour de fonction.
*
*/

  glutInitWindowSize(640, 480);

  /* the window starts at the upper left corner of the screen */

/**	            This Code initialize the position of the window into screen.
*
* \brief      L'appel de la fonction glutInitWindowPosition : fonction Glut initialisant la position de la fenêtre dans l'écran.
* \details    En entrée de cette fonction, les deux paramètres X et Y de positionnement de la fenêtre dans l'écran (deux dimensions)
* \param	  X			    le positionnement de la fenêtre en x
* \param	  Y			    le positionnement de la fenêtre en y
* \return     void          aucun retour de fonction.
*
*/

  glutInitWindowPosition(0, 0);

  /* Open a window */

/**	            This Code create windows on Wayland with glut.
*
* \brief      L'appel de la fonction glutCreateWindow : fonction Glut créant une fenêtre.
* \details    En entrée de cette fonction, l'identification de la fenêtre (.id. son nom).
* \param	  tittle			le nom de la fenêtre
* \return     int               l'identifiant entier de la fenêtre créee.
*
*/

  window = glutCreateWindow("Jeff Molofee's GL Code Tutorial : Draw multicolored triangle and blue square - NeHe '99");

  /* Register the function to do all our OpenGL drawing. */

/**	            This Code rely the internal function DrawGLScene at the display function of glut.
*
* \brief      L'appel de la fonction glutDisplayFunc : fonction Glut permettant d'activer la fonction interne d'affichage.
* \details    En entrée de cette fonction, l'adresse de la fonction interne appellée .
* \param	  &function			l'adresse de la fonction interne d'affichage.
* \return     void              aucun retour de fonction.
*
*/

  glutDisplayFunc(&DrawGLScene);

  /* Even if there are no events, redraw our gl scene. */

/**	            This Code rely the internal function DrawGLScene at the Idle Function of glut.
*
* \brief      L'appel de la fonction glutIdleFunc : fonction d'attente de Glut permettant d'activer la fonction interne d'affichage.
* \details    En entrée de cette fonction, l'adresse de la fonction interne appellée .
* \param	  &function			l'adresse de la fonction interne d'affichage.
* \return     void              aucun retour de fonction.
*
*/

  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */

/**	            This Code rely the internal function ResizeGLScene at the reshape function of glut.
*
* \brief      L'appel de la fonction glutReshapeFunc : fonction Glut permettant d'activer la fonction interne de changement des dimensions d'affichage.
* \details    En entrée de cette fonction, l'adresse de la fonction interne appellée.
* \param	  &function			l'adresse de la fonction interne traitant des changements de dimension de l'affichage.
* \return     void          aucun retour de fonction.
*
*/

  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */

/**	            This Code give pressed key used by this program.
*
* \brief      L'appel de la fonction glutKeyboardFunc : fonction Glut permettant de recupérer la touche appuyée.
* \details    En entrée de cette fonction, l'adresse de la touche appuyée.
* \param	  &key			l'adresse de la touche appuyée sur le clavier.
* \return     void          aucun retour de fonction.
*
*/

  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */

/**	            This Code initialize les paramètres d'affichage OpenGL.
*
* \brief      L'appel de la fonction InitGL : fonction d'initialisation dde la taille de la fenêtre d'affichage OpenGL (la même que celle de glut).
* \details    En entrée de cette fonction, les deux paramètres X et Y correspondant à la taille de la fenêtre OpenGL dans l'écran (deux dimensions)
* \param	  X			    la largeur de la fenêtre en x
* \param	  Y			    la hauteur de la fenêtre en y
* \return     void          aucun retour de fonction.
*
*/

  InitGL(640, 480);

  /* Start Event Processing Engine */

/**	            This Code run the permanently wait loop of events.
*
* \brief      L'appel de la fonction glutMainLoop : fonction lancant la boucle d'attente des événements sous glut.
* \details    Aucun paramètre en entrée ni en sortie.
* \return     void          aucun retour de fonction.
*
*/

  glutMainLoop();

  return 1;
}

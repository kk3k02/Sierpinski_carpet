/*************************************************************************************/

// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>

#include <gl/gl.h>

#include <gl/glut.h>

#include <iostream>

using namespace std;

/*************************************************************************************/


// Funkcja do generowania liczb pseudolosowych
/*************************************************************************************/
int random(int blur_level) {
    if (blur_level == 0) return 0;
    return rand() % blur_level;
}

/*************************************************************************************/

// Funkcja do rysowania dywanu Sierpi�skiego
void drawCarpet(int x, int y, int size, int depth, int blur_level) {
    if (depth == 0) {
        glColor3f(random(blur_level), random(blur_level), random(blur_level));
        // Ustawienie aktualnego koloru rysowania na zielony

        int blur = random(blur_level);
        // Losowanie poziomu rozmycia

        // Narysuj czworokat
        glBegin(GL_POLYGON);
        glVertex2i(x - blur, y - blur);
        glVertex2i(x + size + blur, y + blur);
        glVertex2i(x + size - blur, y + size - blur);
        glVertex2i(x + blur, y + size + blur);
        glEnd();
    }
    else {
        int newSize = size / 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!(i == 1 && j == 1)) {
                    int newX = x + i * newSize;
                    int newY = y + j * newSize;
                    // Rekurencyjnie rysuj mniejsze dywany
                    drawCarpet(newX, newY, newSize, depth - 1, blur_level);
                }
            }
        }
    }

    glFlush();
}

/*************************************************************************************/
// Funkcaja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)

void RenderScene(void)
{

    int max_depth = 0;
    int size = 0;
    int blur_level = 0;

    cout << "Podaj poziom rekursji: ";
    cin >> max_depth;

    size = powf(3, max_depth);

    cout << "Podaj poziom rozmycia: ";
    cin >> blur_level;

    cout << endl;

    glClear(GL_COLOR_BUFFER_BIT);

    // Rozpocznij rysowanie dywanu od punktu (-50, -50)
    drawCarpet(-50.0f, -50.0f, size, max_depth, blur_level);

    glFlush();

}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void)

{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Kolor okna wn�trza okna - ustawiono na czarny

}

/*************************************************************************************/

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;


    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)


    glMatrixMode(GL_PROJECTION);
    // Okre�lenie uk�adu wsp�rz�dnych obserwatora

    glLoadIdentity();
    // Okre�lenie przestrzeni ograniczaj�cej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wsp�czynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // okre�lenie okna obserwatora.
    // Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
    // Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Okre�lenie uk�adu wsp�rz�dnych    

    glLoadIdentity();

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(int argc, char** argv)

{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    // Ustawienie trybu wy�wietlania
    // GLUT_SINGLE - pojedynczy bufor wy�wietlania
    // GLUT_RGBA - model kolor�w RGB



    glutCreateWindow("Dywan Sierpi�skiego z perturbacjami");
    // Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna


    glutDisplayFunc(RenderScene);
    // Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
    // Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
    // trzeba b�dzie przerysowa� okno


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
    // zmiany rozmiaru okna

    MyInit();
    // Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
    // inicjalizacje konieczneprzed przyst�pieniem do renderowania


    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/


/*************************************************************************************/

// Trochê bardziej skomplikowany program wykorzystuj¹cy funkcje biblioteki OpenGL

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

// Funkcja do rysowania dywanu Sierpiñskiego
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
// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)

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

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)

{

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Kolor okna wnêtrza okna - ustawiono na czarny

}

/*************************************************************************************/

// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;


    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)


    glMatrixMode(GL_PROJECTION);
    // Okreœlenie uk³adu wspó³rzêdnych obserwatora

    glLoadIdentity();
    // Okreœlenie przestrzeni ograniczaj¹cej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wspó³czynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // okreœlenie okna obserwatora.
    // Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
    // Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Okreœlenie uk³adu wspó³rzêdnych    

    glLoadIdentity();

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli



void main(int argc, char** argv)

{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    // Ustawienie trybu wyœwietlania
    // GLUT_SINGLE - pojedynczy bufor wyœwietlania
    // GLUT_RGBA - model kolorów RGB



    glutCreateWindow("Dywan Sierpiñskiego z perturbacjami");
    // Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna


    glutDisplayFunc(RenderScene);
    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
    // Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
    // trzeba bêdzie przerysowaæ okno


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
    // zmiany rozmiaru okna

    MyInit();
    // Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
    // inicjalizacje konieczneprzed przyst¹pieniem do renderowania


    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/


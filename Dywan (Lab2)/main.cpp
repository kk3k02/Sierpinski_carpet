#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

using namespace std;

int recursion = 0;
int blur_level = 0;
bool drawn = false; // Flaga �ledz�ca, czy dywan zosta� ju� narysowany

// Funkcja do generowania liczb pseudolosowych
int random(int blur_level) {
    if (blur_level == 0 || blur_level == 1) return 1;
    return rand() % blur_level;
}

// Funkcja do rysowania dywanu Sierpi�skiego
void drawCarpet(int x, int y, int size, int depth, int blur_level) {
    if (depth == 0) {

        glColor3f(random(blur_level), random(blur_level), random(blur_level));
        // Ustawienie aktualnego koloru rysowania na losowy

        // Losowanie poziomu rozmycia
        int blur = 0;
        if (blur_level != 0) blur = random(blur_level);


        // Narysuj czworok�t
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

// Funkcja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)
void RenderScene(void) {
    if (!drawn) {
        int size = powf(3, recursion);

        glClear(GL_COLOR_BUFFER_BIT); // Wyczy�� bufor tylko raz

        // Rozpocznij rysowanie dywanu od punktu (-100, -100)
        drawCarpet(-100.0f, -100.0f, size, recursion, blur_level);

        drawn = true; // Oznacz, �e dywan zosta� ju� narysowany
    }

    glFlush();
}

// Funkcja ustalaj�ca stan renderowania
void MyInit(void) {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    // Kolor okna wn�trza okna - ustawiono na szary
}

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical) {
    GLfloat AspectRatio;

    if (vertical == 0)
        vertical = 1;

    glViewport(0, 0, horizontal, vertical);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

    cout << "Podaj poziom rekursji: ";
    cin >> recursion;

    cout << "\nPodaj poziom perturbacji: ";
    cin >> blur_level;

    // Oblicz now� warto�� `size` na podstawie wzoru size = powf(3, recursion) - 100.00
    float size = powf(3, recursion) - 100.0;

    if (horizontal <= vertical)
        glOrtho(-100.0, size, -100.0 / AspectRatio, size / AspectRatio, 1.0, -1.0);
    else
        glOrtho(-100.0 * AspectRatio, size * AspectRatio, -100.0, size, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
    // inicjalizacje konieczne przed przyst�pieniem do renderowania

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

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT

    return 0;
}

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
#include <bits/stdc++.h> 
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
bool m[20][20]={};
int f,c;
struct perceptron{
	int v[10];
	vector<double> pesos[10];
	double b;
	void extraerDatos(int i, vector<vector<bool> > &matricesEntrenamiento, vector<int> &correspondiente){
		v[i]=0;
		ifstream ifs;
		ifs.open(to_string(i)+".txt", std::ifstream::in);
		string a;
		ifs>>a;
		while (ifs.good()) {
			int n= stoi(a);
			ifs>>a;
			int m= stoi(a);
			if(n==f && m==c){
				v[i]++;
				matricesEntrenamiento.push_back(vector<bool>(f*c,0));
				correspondiente.push_back(i);
				for(int j=0;j<n;++j){
					ifs>>a;
					cout << a<<endl;
					for(int k=0;k<m;++k){
						matricesEntrenamiento.back()[j*m+k]=(a[k]=='1');
					}
				}
			}else{
				for(int j=0;j<n;++j){
					ifs>>a;
				}
			}
			ifs>>a;
		}
		ifs.close();
	}
	void entrenar(double tasaAprendizaje){
		b=0.0;
		for(int i=0;i<10;++i){
			pesos[i].resize(f*c,0.0);
		}
		vector<vector<bool> > matricesEntrenamiento;
		vector<int> correspondiente;
		for(int i=0;i<10;++i){
			extraerDatos(i,matricesEntrenamiento,correspondiente);
		}
		for(int i=0;i<10;++i){
			if(v[i]>0){
				bool algunoNoCoincide=true;				
				while(algunoNoCoincide){
					algunoNoCoincide=false;
					for(auto it=0;it<matricesEntrenamiento.size();it++){
						double resultado=b;
						for(int j=0;j<matricesEntrenamiento[it].size();++j){
							resultado+=matricesEntrenamiento[it][j]*pesos[i][j];
						}
						bool y=(resultado>0);
						if(y!=(i==correspondiente[it]) ){
							algunoNoCoincide=true;
							int diferencia=1;
							if(y)
								diferencia=-1;
							b+=diferencia;
							for(int j=0;j<matricesEntrenamiento[it].size();++j){
								pesos[i][j]+=diferencia*tasaAprendizaje*matricesEntrenamiento[it][j];
							}
						}
					}	
				}
				cout<<i<<" Entrenado";
			}
		}
	}
	vector<int> reconocer(){
		vector<int> posibles;
		for(int k=0;k<10;++k){
			if(v[k]>0){
				double resultado=b;
				for(int i=0;i<f;++i){
					for(int j=0;j<c;++j){
						resultado+=pesos[k][i*c+j]*m[i][j];
					}
				}
				if(resultado>0)
					posibles.push_back(k);
			}
		}
		return posibles;
	}
};
perceptron x;
void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i=0;i<f;++i){
		for (int j=0;j<c;++j){
			glColor3f(0.0f, 0.0f, 1.0f); glLineWidth(3);
			glBegin(GL_POLYGON);
			glVertex2d(70*j,70*i);
			glVertex2d(70*(j+1),70*i);
			glVertex2d(70*(j+1),70*(i+1));
			glVertex2d(70*j,70*(i+1));
			glEnd();
			if(!m[f-i-1][j]){
				glBegin(GL_POLYGON);
				glColor4f(1.0f, 1.0f, 0.0f, 0.0f); glLineWidth(3);
				glVertex2d(70*j+2,70*i+2);
				glVertex2d(70*(j+1)-2,70*i+2);
				glVertex2d(70*(j+1)-2,70*(i+1)-2);
				glVertex2d(70*j+2,70*(i+1)-2);
				glEnd();
			}
		}
	}
	vector<int> posibles=x.reconocer();
	glColor3f(1.0f, 0.5f, 0.0f); glLineWidth(3);
	for(int i=0;i<posibles.size(); ++i){
		cout<<posibles[i]<<endl;
		glRasterPos3f(70*c+10, (f*35)-i*50, 0);
		string a="reconoce: " +to_string(posibles[i]);
		for (int j = 0; j < a.length(); j++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,a[j]);
		}
	}
	glutSwapBuffers();
}

//funcion para hacerle click a la matriz
GLvoid OnMouseClick (int button, int state, int x, int y) {
	if ( (button == GLUT_LEFT_BUTTON or button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN ) {		
		x/=70;
		y/=70;
		m[y][x]= !m[y][x];
	}
	glutPostRedisplay ();
}

//funciones para incializar la pantalla
void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (c*70+200,f*70);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("v");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glutMouseFunc (&OnMouseClick);
	glClearColor(2.f,2.f,2.f,2.f);
}
//mi menu donde inicion filas y columnas
int main (int argc, char **argv) {
	cout<<"filas: ";
	cin>>f;
	cout<<"columnas: ";
	cin>>c;
	x.entrenar(0.5);
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}

 
char achar[5]={'a','b','c','d','e'};
int aint[5]={0, 1, 2, 3, 4};
float afloat[5]={1.1, 2.2, 3.3, 4.4, 5.5};
 
void main(){
  int i=0;

  cout << "Los 5 primeros caracteres son:\n";
  while (i < 5) {
  cout << achar[i] << "\n";
  i = i + 1;
  }
 
  cout << "Los 5 primeros enteros son:\n";
  i=0;
  while (i < 5) {
  cout << aint[i] <<" \n";
  i = i + 1;
  }
  
  cout << "5 numeros flotantes son";
  i=0;
  while (i < 5) {
  cout << afloat[i] << "\n";
  i = i + 1;
  }
 
  afloat[0]=achar[0];
  afloat[1]=aint[0];
  aint[0]=achar[0];
 
  cout << "la letra a vista como flotante:  "<< afloat[0]<< "\n";
  cout << "el entero 0 visto como flotante:  "<< afloat[1] << "\n"; 
  cout << "la letra a vista como entera:  "<< aint[0] <<" \n";
 }
 



 

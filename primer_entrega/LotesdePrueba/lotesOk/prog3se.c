

int i, dato;
int a[10];
 
 
 
 
 

int fcontrol() {
  if ((dato < -32000) || (dato > +32000)) {
  cout << "El valor entero  "<< dato <<  "esta fuera de rango";
  return 0;
  } 
  else
 return 1; 
 }
 
 
void fcarga (int vec[]) {
 
  int a;
  int c;
  
  a = 1;
  while (a <= 10) {
     c = 0;
     while (! c){
       cout << "ingrese un valor entero: ";
        cin >> dato;
        c = fcontrol();
     } 
    vec[a-1] = dato;
    a = a + 1; 
  }
}

 void main() {

 
 fcarga(a);


  i = 1;
  while (i < 35) {
  cout << ;
  i = i + 1;
  }


  i = 0;
 while (i < 10){
  cout << "El entero:  "<< a[i] << "esta en la posicion:  "<< i; 
  i = i + 1;
 }
 
}

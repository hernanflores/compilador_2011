 
 void fcalc (char vec[], int lim, char & a)
 {
 
  if(lim > 0){
  vec[lim - 1]=a;
  cout << vec[lim - 1];
  fcalc(vec,lim-1,a);
  }
 
 }
 
 void main () {
  char v[20];
  char a='A';
 
  fcalc (v, 10, a);
 
 }

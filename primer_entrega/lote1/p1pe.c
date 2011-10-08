int a, b;
char b;
# error, ya definida # 

void main() {
	noesta = 4;
# error la variable no esta definida #
	b = 4 + 10;
	b = noesta + 1;
# noesta tendria que haber sido insertado en TS no tendria que dar error #
	noesta = 18 * b;
	b = noesta[1];
}


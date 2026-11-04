#include <stdio.h>
#include <math.h>
#include <locale.h>


int calcula_raizes(float a, float b, float c, float *x1, float *x2) {
	float delta;
	
	delta = (b * b) - (4 * a * c);
	printf("O valor do Delta é: %.2f\n", delta);

	if (delta < 0) {
		return 0;
	}
	else if (delta == 0) {
		*x1 = -b / (2 * a);
		*x2 = *x1;
		return 1;
	}
	else {
		*x1 = (-b + sqrt(delta)) / (2 * a);
		*x2 = (-b - sqrt(delta)) / (2 * a);
		return 2;
	}
	
}

int main() {
	float a, b, c, x1, x2;
	int resultado;
	setlocale(LC_ALL, "Portuguese");

	printf("Digite os valores de a, b e c: ");
	scanf("%f %f %f", &a, &b, &c);

	resultado = calcula_raizes(a, b, c, &x1, &x2);

	if (resultado == 0) {
		printf("A equação não possui raizes reais.\n");
	}
	else if (resultado == 1) {
		printf("A equação possui uma raiz real: %.2f\n", x1);
	}
	else {
		printf("A primeira raiz é: %.2f\n", x1);
		printf("A segunda raiz é: %.2f\n", x2);
	}

	return 0;
}

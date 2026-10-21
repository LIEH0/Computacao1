#include <stdio.h>
#include <locale.h>

int calculaDigitoVerificador(int num);

int main()
{

	int numero;
	printf ("Digite um número: ");
	scanf("%d", &numero);

	printf("O dígito verificador é: %d", calculaDigitoVerificador(numero));


	return 0;
}

int calculaDigitoVerificador(int num)
{
	int x1, x2, x3, x4;
	int soma;



	x1 = num/100;
	x2 = (num/10)%10;
	x3 = num%10;
	x4 = (((1*x1 + 2*x2 + 3*x3))%11)%10;

	// 	printf("%d\n", x1);
	// 	printf("%d\n", x2);
	// 	printf("%d\n", x3);


	return x4;
}

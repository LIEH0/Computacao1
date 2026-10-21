#include <stdio.h>
#include <locale.h>

float calculaIMC(float peso, float altura);
char verificarSituacao(float peso, float altura);

int main()
{
	setlocale(LC_ALL, "Portuguese");

	float peso, altura, imc;
	char situacao;

	printf("  SISTEMA DE CALCULO DE IMC  \n\n");
	printf("\nDigite o peso do paciente (kg): ");
	scanf("%f", &peso);

	printf("Digite a altura do paciente (metros): ");
	scanf("%f", &altura);

	while (peso > 0 && altura > 0)
	{
		imc = calculaIMC(peso, altura);
		situacao = verificarSituacao(peso, altura);

		printf("\nIMC: %.2f\n", imc);
		printf("Situacao: %c\n", situacao);

		printf("\nDigite o peso do paciente (kg): ");
		scanf("%f", &peso);

		printf("Digite a altura do paciente (metros): ");
		scanf("%f", &altura);
	}

	printf("\nEncerrando...\n");

	return 0;
}

float calculaIMC(float peso, float altura)
{
	return peso / (altura*altura);
}

char verificarSituacao(float peso, float altura)
{
	float imc;

	imc = calculaIMC(peso, altura);

	if (imc < 18.5)
	{
		return 'B';
	}
	else if (imc < 25.0)
	{
		return 'N';
	}
	else if (imc < 30.0)
	{
		return 'S';
	}
	else if (imc < 35.0)
	{
		return 'O';
	}
	else if (imc <= 40.0)
	{
		return '1';
	}
	else
	{
		return '2';
	}
}

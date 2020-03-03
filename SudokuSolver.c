#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_sort[9];
int count[9];

int **alocar_memoria (int i, int j)
{
    int **p;
    int aux;
    if (i < 1 || j <1)
    {
        printf("\nUm erro ocorreu ao alocar a memoria. Verifique todos os parametros e tente novamente.");
        exit(0);
    }

    p = (int **) calloc (i, sizeof(int *));

        if (p == NULL)
    {
        printf("\nUm erro ocorreu ao alocar a memoria. Verifique todos os parametros e tente novamente.");
        exit(0);
    }

    for (aux = 0; aux < i; aux++ )
    {
        p[aux] = (int*) calloc (j, sizeof(int));

        if (p[aux] == NULL)
        {
            printf("\nUm erro ocorreu ao alocar a memoria. Verifique todos os parametros e tente novamente.");
            exit(0);
        }
    }

    return p;
}

void sort(int add, int i, int j, int **sudoku)
{
    int aux1[9], aux2[9]={1,2,3,4,5,6,7,8,9}, n, temp1, temp2;
    if(add) count[add]++;

    else
    {
        add = sudoku[i][j];
        count[add]--;
    }

    for(n=0; n < 9; n++)
    {
        aux1[n]=count[n];
    }

    for(n=0; n < 8; n++)
    {
        if(aux1[n]<aux1[n+1])
        {
            temp1 = aux1[n];
            aux1[n]= aux1[n+1];
            aux1[n+1] = temp1;

            temp2 = aux2[n];
            aux2[n]= aux2[n+1];
            aux2[n+1] = temp2;
        }
    }
    for(n=0; n < 9; n++)
    {
        num_sort[n]=aux2[n];
    }
}

int **input_matriz()
{
    int **m, i, j, k;
    char name[80];
    FILE *input;

    m = alocar_memoria(9,9);

    printf("Digite o nome do arquivo com o problema (favor nao incluir o.txt). Arquivo não deve conter espacos entre os numeros de mesma linha e deve ser 9x9.");
    gets(name);
    strcat(name, ".txt");

    input = fopen(name, "r");

    if(!input)
    {
        printf("\nUm erro ocorreu ao carregar o arquivo. Verifique todos os parametros e tente novamente. [1]");
        exit(0);
    }

        for (i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
        {
            fscanf(input, "%1d", &k);
            //k = k - 48; //Passa de ASCII para inteiro.
            printf("%d\n", k);

            if ( (k >= 0 ) & ( k <= 9) )
            {
                m[i][j] = k;

                if(k)
                {
                    printf("%d, %d", k, k-1);
                    sort(k-1, 0, 0, m);
                }

            }
            else
            {
                printf("\nUm erro ocorreu ao carregar o arquivo. Verifique todos os parametros e tente novamente. %d %d [2]", i, j);
                exit(0);
            }

                if (k == ' ')
                {
                    printf("\nUm erro ocorreu ao carregar o arquivo. Verifique todos os parametros e tente novamente. [3]");
                    exit(0);
                }

                if ((k == EOF) & ( (j!=8) & (i!=8) ) )
                {
                    printf("\nUm erro ocorreu ao carregar o arquivo. Verifique todos os parametros e tente novamente. [4]");
                    exit(0);
                }
        }
    }

    fclose(input);
    return m;
}

int teste_presenca(int **sudoku, int linha, int coluna, int num)
{
    int linha_inicial = (linha/3) * 3;
    int coluna_inicial = (coluna/3) * 3;
    int i;

    for(i=0; i<9; i++)
    {
        if(sudoku[linha][i] == num) return 0;

        if(sudoku[i][coluna] == num) return 0;

        if(sudoku[linha_inicial + (i%3)][coluna_inicial + (i/3)] == num) return 0;

    }

    return 1;
}

int sudokusolver(int **sudoku, int linha, int coluna)
{
    int num, i;
    if(linha <9 && coluna <9)
    {
        if(sudoku[linha][coluna])
        {
            if((coluna+1) < 9) return sudokusolver(sudoku, linha, coluna+1);

            else if((linha)<9) return sudokusolver(sudoku, linha+1, 0);

            else return 1;

        }

        else
        {
            for(i=0; i<9; i++)
            {
                num = num_sort[i];

                if(teste_presenca(sudoku, linha, coluna, num))
                {
                    sudoku[linha][coluna] = num;
                    sort(num,linha, coluna, sudoku);
                    if((coluna+1)<9)
                    {
                        if(sudokusolver(sudoku, linha, coluna+1)) return 1;

                        else
                        {
                            sudoku[linha][coluna] = 0;
                            sort(0, linha, coluna, sudoku);
                        }


                    }

                    else if((linha+1)<9)
                    {
                        if(sudokusolver(sudoku, linha+1, 0)) return 1;

                        else
                        {
                            sudoku[linha][coluna] = 0;
                            sort(0, linha, coluna, sudoku);
                        }

                    }

                    else return 1;
                }
            }
        }

        return 0;
    }

    else return 1;
}

void output(int **output)
{
    FILE *out;
    int i, j;

    out = fopen("Resultado.txt", "w");

    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            putc((output[i][j]+48), out);
        }
        putc('\n', out);
    }

    fclose(out);
    printf("\n\tSudoku resolvido. Resultado disponível no arquivo Resultado.txt.\n");
}

void mostrar_sudoku(int **sudoku)
{
    int i, j;
    printf("_________\n|");
        for(i=0; i<9; i++)
            {
                for(j=0; j<9; j++)
                {
                    printf("%d",sudoku[i][j]);
                    if(j%3 == 2) printf("|");
                }

                printf("\n|");

                if(i%3 == 2) printf("_________\n|");
            }
}

int main()
{
    int **sudoku;

    printf("\n\n\tPrograma Solver de Sudoku\n\n");

    sudoku = input_matriz();

    mostrar_sudoku(sudoku);

    if (sudokusolver(sudoku, 0, 0))
    {
      output(sudoku);
      mostrar_sudoku(sudoku);
    }

    else printf("\nUm erro ocorreu ao tentar resolver o problema. Verifique todos os parametros e tente novamente.");

    return 0;
}

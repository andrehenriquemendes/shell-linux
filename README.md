# shell-linux
Implementação de um shell que reconhece comandos simples e comandos encadeados com pipe "|".</br>
Desenvolvido para a disciplina de Sistemas Operacionais na Universidade Federal de São Paulo.
</br>
## Relatório completo disponível em:
https://drive.google.com/file/d/1yT_tqJ1DU5EsFO_cCk5Ahj38Oz5VZL2b/view?usp=sharing
</br></br>
## Relatório enxuto para rodar a aplicação:

### Para compilar o programa:
gcc -Wall -o shell shell.c
</br>
</br>
### Para rodar a aplicação
Comando simples
- ./shell "comando"

Exemplo:

- *./shell ls*
</br>
Para utilizar o pipe, para encadear comandos simples, utilizar: "|"

Exemplo:

- *./shell ls "|" grep "txt"*


import sys
from random import randint

tempo = ['sol', 'vento', 'chuva']
parentes = ['sim', 'nao']
dinheiro = ['muito', 'pouco']
dia = ['domingo', 'segunda', 'terca', 'quarta', 'quinta', 'sexta', 'sabado']
mes = ['janeiro', 'fevereiro', 'marco', 'abril', 'maio', 'junho', 'julho', 'agosto', 'setembro', 'outubro', 'novembro', 'dezembro']
decisao = ['cinema', 'kart', 'casa', 'shopping']

atributos = [tempo, parentes, dinheiro, dia, mes, decisao]

linhas = 1000000

for i in range(linhas):
    for j in  range(len(atributos)):
        lenght = (len(atributos[j]))-1
        random = randint(0, lenght)
        sys.stdout.write(atributos[j][random])
        if j < (len(atributos)-1):
            sys.stdout.write(",")
    sys.stdout.write('\n')
